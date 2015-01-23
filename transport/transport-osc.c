#include "lo/lo.h"
 

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


int quit_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	quit(trans);
	//trans->run = 0;
	return 0;
}


int start_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	start(trans);
	//trans->on = 1;
	return 0;
}

int stop_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	//trans->on = 0;
	//send_stop_msg(trans);
	stop(trans);
	return 0;
}

//int tap_handler(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);
//int clear_tap_handler(const char *path, const char *types, lo_arg ** argv,
//		                    int argc, void *data, void *user_data);

int set_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = bpm_to_period( argv[0]->f);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm_msgs(trans);
}

int inc_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = 
		bpm_to_period(period_to_bpm(trans->tick_period) + 1);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm_msgs(trans);
}

int dec_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	struct timespec period = 
		bpm_to_period(period_to_bpm(trans->tick_period) - 1);
	if(validate_period(period))
			trans->tick_period = period;
	send_bpm_msgs(trans);
}

//int factor_bpm_handler(const char *path, const char *types, lo_arg ** argv,
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
void send_tick_msgs(struct transport * trans)
{
	puts("tick!");
	lo_send(trans->tick_client_list->client->addr, "/sc/transport/tick", "i",
		trans->tick);
}

void send_bpm_msgs(struct transport * trans)
{
	printf("bpm: %f\n", period_to_bpm(trans->tick_period));
}

void send_stop_msgs(struct transport * trans)
{
	puts("stop!");
}
