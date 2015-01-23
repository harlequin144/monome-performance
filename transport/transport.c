#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
 

#include "time.c"
#include "transport.h"
#include "transport-osc.h"
#include "transport-osc.c"
 

// Notes
// - Period refers to the time between ticks
// - bpm is just regular bpm
//
//
// to do
// - make sure the timing is robust before moving on!!!


int main(void)
{
	// Read Args
	// Read config
	// forkGui
	// forkMonome

// Setup
	struct transport * trans;


	trans = new_transport();
	start_transport_loop(trans);

	// clean up
	printf("In successfully\n");
	exit(EXIT_SUCCESS);
}


void start_transport_loop(struct transport * trans)
{
	int return_code;
	struct timespec elapsed_time;
	struct timespec current_time;

	while( trans->run ){

		if( trans->on ){
			return_code = clock_gettime( CLOCK_MONOTONIC, &current_time );
			assert(return_code == 0);

			elapsed_time = timespec_norm(current_time, trans->last_tick_time);

			if( check_tick_expired(elapsed_time, trans->tick_period) ){
				trans->last_tick_time = current_time;
				send_tick_msgs(trans);
				trans->tick = ((trans->tick + 1)%TICKS_PER_BEAT);
			}
		}

		lo_server_recv_noblock(trans->osc_server, 1);
	}
}


struct transport * new_transport()
{
	struct transport * trans;
	trans = (struct transport*) calloc(1, sizeof(struct transport));
	trans->run = 1;
	trans->on = 1;
	trans->tick = 0;


	int return_code = 0;
	return_code = clock_gettime(CLOCK_MONOTONIC, &(trans->last_tick_time));
	assert(return_code == 0);

	trans->tick_period = bpm_to_period(120);


	trans->osc_server = lo_server_new("8001", error);
	if(!(trans->osc_server)){
		puts("Exiting. failed to make ther osc server");
		exit(1);
	}

	lo_server_add_method(trans->osc_server, "/transport/quit",  NULL,
			quit_handler, &trans);
	lo_server_add_method(trans->osc_server, "/transport/start", NULL,
			start_handler, &trans);
	lo_server_add_method(trans->osc_server, "/transport/stop", NULL, 
			stop_handler, &trans);
	//lo_server_add_method(trans->osc_server, "/transport/tap", NULL,
			//tap_handler, &trans);
	//lo_server_add_method(trans->osc_server, "/transport/clear_tap",
			//clear_tap_handler, NULL);
	lo_server_add_method(trans->osc_server, "/transport/set_bpm", "f",
			set_bpm_handler, &trans);
	lo_server_add_method(trans->osc_server, "/transport/inc_bpm", NULL,
			inc_bpm_handler, &trans);
	lo_server_add_method(trans->osc_server, "/transport/dec_bpm", NULL,
			dec_bpm_handler, &trans);
	//lo_server_add_method(trans->osc_server,"/transport/factor_bpm_handler",
			//"f", factor_period, &trans);
	//lo_server_add_method(trans->osc_server,"/transport/reg_bpm_rcvr_handler",
			//"is", reg_bpm_rcvr, &trans);
	//lo_server_add_method(trans->osc_server,"/transport/reg_tick_rcvr_handler",
			//"is", reg_tick_rcvr, &trans);
			//
	lo_server_add_method(trans->osc_server,NULL, NULL, generic_handler, NULL);


	//lo_address bridge_send;
	//bridge_send = lo_address_new(NULL, "8000");


	struct client * supercollider;
	supercollider = (struct client*) calloc(1, sizeof(struct client));
	supercollider->addr = lo_address_new(NULL, "57120");
	supercollider->path = "/sc/transport/tick";

	struct client_list_node * node;
	node = (struct client_list_node*) calloc(1, sizeof(struct client_list_node));

	node->client = supercollider;
	node->next = NULL;
	
	trans->tick_client_list = node;


	puts("here");
	return trans;
}



/*
 * Basic functions affecting the transport
 * these are the functions that actually touch the timing loop, so all
 * validation happens here.
 */

void quit(struct transport * trans)
{
	trans->run = 0;
}


void start(struct transport * trans)
{
	trans->on = 1;
}

void stop(struct transport * trans)
{
	trans->on = 0;
	send_stop_msgs(trans);
}

int set_bpm(struct transport * trans, float bpm)
{
	struct timespec period = bpm_to_period( bpm );
	if( validate_period(period) ){
			trans->tick_period = period;
		send_bpm_msgs(trans);
	}
}

//int tap(const char *path, const char *types, lo_arg ** argv,
//int clear_tap(const char *path, const char *types, lo_arg ** argv,
//int factor_period(
