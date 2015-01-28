#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "lo/lo.h"



#define TICKS_PER_BEAT 144 // two factors of 3 to allow for triplets. :)
#define NANOS_PER_SEC 1000000000

const struct timespec MIN_PERIOD= {.tv_sec = 0, .tv_nsec = 1040000};
const struct timespec MAX_PERIOD= {.tv_sec = 0, .tv_nsec = 21000000};
const struct timespec TOL = {.tv_sec = 0, .tv_nsec = 100};
const struct timespec SLEEP_TIME = {.tv_sec = 0, .tv_nsec = 100000};

const struct sched_param SCHED_PARAM = {.sched_priority = 90};



struct transport{
	char run;
	char on;

	int tick;
	
	struct timespec last_tick_time; 
	struct timespec tick_period; 

	lo_server osc_server;
	lo_address monome_address;
	//lo_address gui_address;

	struct client_list_node * tick_client_list;
	struct client_list_node * bpm_client_list;
};

struct transport_params{
	char * transport_port;
	char * gui_port;
	char * monome_port;
	char * bridge_port;

	struct client_list_node * tick_client_list;
	struct client_list_node * bpm_client_list;
};


struct client{
	lo_address addr;
	char * prefix;
};
 
struct client_list_node{
	struct client * client;
	struct client_list_node * next;
};



void start_transport_loop(struct transport * tran);
struct transport * new_transport();

void read_config(struct transport_params * params);
enum lcfg_status 
config_iterator(const char * key, void * data, size_t len, void * user_data);

void print_client_list(struct client_list_node * list);
void add_client(struct client_list_node ** list, char ** port, char ** prefix);

// The basic functions of the transport
void quit(struct transport * trans);
void set_loop_on(struct transport * trans);
void set_loop_off(struct transport * trans);
int set_bpm(struct transport * trans, double bpm);
//int tap(struct transport * trans)
//int clear_tap(struct transport * trans)

void reg_bpm_client(struct transport * trans, char * prefix, char * port);
void reg_tick_client(struct transport * trans, char * prefix, char * port);




//#ifndef TRANSPORT_OSC_H
//#define TRANSPORT_OSC_H

//#include "lo/lo.h"


// The basic osc handlers 
int start_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int stop_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);
int toggle_handler(const char *path, const char *types, lo_arg ** argv,
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


int reg_bpm_rcvr_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int reg_tick_rcvr_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int monome_press_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);
int monome_show_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);
int monome_hide_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data);


// osc server error function
void error(int num, const char *msg, const char *path);


// Outgoing Osc
void send_tick_msgs(struct transport * trans);
void send_bpm_msgs(struct transport * trans);
void send_stop_msgs(struct transport * trans);

//#endif

#endif
