#ifndef TRANSPORT_H
#define TRANSPORT_H
//#include <pthread.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <time.h>
 
#include "lo/lo.h"
#define TICKS_PER_BEAT 144 // two factors of 3 to allow for triplets. :)
#define NANOS_PER_SEC 1000000000
#define TOL 100000

struct transport{
	char run;
	char on;
	
	struct timespec last_tick_time; 
	struct timespec tick_period; 

	lo_server osc_server;
};
 

void run_main_loop(struct transport * tran);
void check_time(struct transport * tran);


// Time Operations 
struct timespec timespec_norm(struct timespec time1, struct timespec time2);
struct timespec bpm_to_period(float bpm);
float 					period_to_bpm(struct timespec period);
void						print_timespec(struct timespec time);

int	validate_bpm(float bpm);
int	validate_period(struct timespec period);


// The basic functions of the clock
int start(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int stop(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int quit(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int tap(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int clear_tap(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int set_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int inc_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int dec_bpm(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int factor_period(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int reg_bpm_rcvr(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int reg_tick_rcvr(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);


// osc server error function
void error(int num, const char *msg, const char *path);


// Outgoing Osc
void send_tick(struct transport * trans);
void send_bpm(struct transport * trans);
void send_stop(struct transport * trans);

#endif
