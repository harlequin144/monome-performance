#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "lo/lo.h"



#define TICKS_PER_BEAT 144 // two factors of 3 to allow for triplets. :)
#define NANOS_PER_SEC 1000000000


const struct sched_param SCHED_PARAM = {.sched_priority = 50};

const struct timespec MIN_PERIOD = {.tv_sec = 0, .tv_nsec = 1040000};
const struct timespec MAX_PERIOD = {.tv_sec = 0, .tv_nsec = 21000000};

const struct timespec TOL = {.tv_sec = 0, .tv_nsec = 100};
const struct timespec TAP_TIMES_EXPIRE = {.tv_sec = 6, .tv_nsec = 0};

//const struct timespec SLEEP_TIME = {.tv_sec = 0, .tv_nsec = 100000};








struct transport
{
	char run;
	char on;

	int tick;
	
	struct timespec last_tick_time; 
	struct timespec tick_period; 

	lo_server osc_server;
	lo_address monome_address;

	struct client_list_node * tick_clients;
	struct client_list_node * bpm_clients;

	struct tap_list_node * tap_times;
	// This list will hold the most recent tap time in the head, and then as it
	// is updated the values are changed to the tap time periods.
};

struct transport_params
{
	char transport_port[6];
	char monome_port[6];
	char bridge_port[6];

	struct client_list_node * tick_clients;
	struct client_list_node * bpm_clients;
};

struct client_list_node
{
	lo_address addr;
	char * prefix;
	struct client_list_node * next;
};

struct tap_list_node
{
	struct timespec time;
	struct tap_list_node * next;
};




// Initialization and Startup
void parse_config(struct transport_params * params);
enum lcfg_status config_iterator(
		const char * key, void * data, size_t len, void * user_data);

void 	new_transport(
		struct transport * trans, struct transport_params * params);
void 	start_transport_loop(struct transport * trans );
int 	check_tick_expired(struct timespec elapsed, struct timespec period);

void 	add_tick_client_param( struct transport_params * trans, char * port, 
		char * prefix );
void 	add_bpm_client_param( struct transport_params * trans, char * port, 
		char * prefix );


// Primary Transport Methods
void 	quit( struct transport * trans );
void 	set_loop_on( struct transport * trans );
void 	set_loop_off( struct transport * trans );
void 	set_tick_period( struct transport * trans, struct timespec period );
//void 	add_client( struct client_list_node ** head, char * port, char* prefix );
void 	add_tick_client( struct transport * trans, char * port, char * prefix );
void 	add_bpm_client( struct transport * trans, char * port, char * prefix );
void 	tap( struct transport * trans, struct timespec tap_time );

void calculate_tick_period( struct transport * trans );

void 	clear_tap( struct transport * trans );

// Secondary Methods - based on the basic ones
void 	set_bpm( struct transport * trans, double bpm );

void 	print_tap_times( struct tap_list_node * node );
void  print_client_list( struct client_list_node ** trans);
void 	print_timespec( struct timespec time);


// Time Functions
int timespec_geq(struct timespec time1, struct timespec time2);
int timespec_leq(struct timespec time1, struct timespec time2);
double timespec_to_double(struct timespec time);
struct timespec double_to_timespec(double time);
struct timespec timespec_norm(struct timespec time1, struct timespec time2);

double period_to_bpm(struct timespec period);
struct timespec bpm_to_period(double bpm);


//
// The osc handlers 
//

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


int add_bpm_client_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int add_tick_client_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);

int forward_press_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);
int forward_show_handler(const char *path, const char *types, lo_arg **
		argv, int argc, void *data, void *user_data);
int forward_hide_handler(const char *path, const char *types, lo_arg **
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
