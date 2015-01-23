#ifndef TRANSPORT_OSC_H
#define TRANSPORT_OSC_H

#include "lo/lo.h"


// The basic osc handlers 
int start_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int stop_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int quit_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int tap_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int clear_tap_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int set_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int inc_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int dec_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int factor_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);

int reg_bpm_rcvr_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int reg_tick_rcvr_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);


// osc server error function
void error(int num, const char *msg, const char *path);


// Outgoing Osc
void send_tick_msgs(struct transport * trans);
void send_bpm_msgs(struct transport * trans);
void send_stop_msgs(struct transport * trans);

#endif
