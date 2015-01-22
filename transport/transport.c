#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
 
#include "transport.h"
#include "time.c"
 

//
// to do
// - separate the osc functions
// - make sure the timing is robust before moving on!!!
//

int main(void)
{
	// Read Args
	// Read config
	// forkGui
	// forkMonome


	// Setup
	struct transport trans;

	trans.run = 1;
	trans.on = 1;

	trans.osc_server = lo_server_new("8001", error);
	if(!(trans.osc_server)){
		puts("Exiting. failed to make ther osc server");
		exit(1);
	}

	lo_server_add_method(trans.osc_server, "/transport/quit",  NULL, quit,
			&trans);
	lo_server_add_method(trans.osc_server, "/transport/start", NULL, start, 
			&trans);
	lo_server_add_method(trans.osc_server, "/transport/stop", NULL, stop,
			&trans);
	//lo_server_add_method(trans.osc_server, "/transport/tap", NULL, tap,
		//&trans);
	//lo_server_add_method(trans.osc_server, "/transport/clear_tap", clear_tap,
			//stop, NULL);
	lo_server_add_method(trans.osc_server, "/transport/set_bpm", "f", set_bpm,
			&trans);
	lo_server_add_method(trans.osc_server, "/transport/inc_bpm", NULL, inc_bpm,
			&trans);
	lo_server_add_method(trans.osc_server, "/transport/dec_bpm", NULL, dec_bpm,
			&trans);
	//lo_server_add_method(trans.osc_server,"/transport/factor_period", "f",
			//factor_period, &trans);
	//lo_server_add_method(trans.osc_server,"/transport/reg_bpm_rcvr", "is",
			//reg_bpm_rcvr, &trans);
	//lo_server_add_method(trans.osc_server,"/transport/reg_tick_rcvr", "is",
			//reg_tick_rcvr, &trans);
	lo_server_add_method(trans.osc_server,NULL, NULL, generic_handler, NULL);


	//lo_address bridge_send;
	//bridge_send = lo_address_new(NULL, "8000");


	int return_code = 0;

	return_code = clock_gettime(CLOCK_MONOTONIC, &(trans.last_tick_time));
	assert(return_code == 0);

	trans.tick_period = bpm_to_period(120);

	run_main_loop(&trans);

	// clean up
	printf("In successfully\n");
	exit(EXIT_SUCCESS);
}


void run_main_loop(struct transport * trans)
{
	while(trans->run){
		if(trans->on){
			if(check_tick_expired(trans))
				send_tick(trans);
		}
		lo_server_recv_noblock(trans->osc_server, 1);
	}
}

void check_time(struct transport * trans){
	struct timespec current_time;
	struct timespec elapsed_time;

	int return_code;

	return_code = clock_gettime(CLOCK_MONOTONIC, &current_time);
	assert(return_code == 0);

	elapsed_time = timespec_norm(trans->last_tick_time, current_time);
	//printf("elapsed: \n\t %li \n\t %li\n", elapsed_time.tv_sec, 
			//elapsed_time.tv_nsec);

	if( (elapsed_time.tv_sec >= trans->tick_period.tv_sec) && 
			(elapsed_time.tv_nsec >= trans->tick_period.tv_nsec) )
	{
		//send_tick();
		trans->last_tick_time = current_time;
		//puts("late");

	}

	else{
		struct timespec delta;
		delta = timespec_norm(elapsed_time, trans->tick_period);
		if((delta.tv_sec == 0) && (delta.tv_nsec <= TOL)){
			//send_tick();
			trans->last_tick_time = current_time;
			//puts("early");
			//printf("elapsed: \n\t %li \n\t %li\n", norm2.tv_sec, norm2.tv_nsec);
		}
	}
}




/*
 * OSC Handlers
 */

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	puts("generic");
	int i;

	printf("path: <%s>\n", path);
	for (i = 0; i < argc; i++) {
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
	return 1;
}


int quit(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	trans->run = 0;
	return 0;
}


int start(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	trans->on = 1;
	return 0;
}

int stop(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	trans->on = 0;
	send_stop(trans);
	return 0;
}

//int tap(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);
//int clear_tap(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);

int set_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = bpm_to_period( argv[0]->f);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm(trans);
}

int inc_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = 
		bpm_to_period(period_to_bpm(trans->tick_period) + 1);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm(trans);
}

int dec_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = 
		bpm_to_period(period_to_bpm(trans->tick_period) - 1);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm(trans);
}

//int factor_period(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);
//int decrement(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);
//


// osc server error func
void error(int num, const char *msg, const char *path){
	printf("loblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


//
// Outgoing OSC 
//
//
void send_tick(struct transport * trans)
{
	puts("tick!");
}

void send_bpm(struct transport * trans)
{
	printf("bpm: %f\n", period_to_bpm(trans->tick_period));
}

void send_stop(struct transport * trans)
{
	puts("stop!");
}
