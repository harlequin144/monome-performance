#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "lo/lo.h"


#define TICKS_PER_BEAT 144 // two factors of 3 to allow for triplets. :)
#define NANOS_PER_SEC 1000000000
#define TOL 100000


const struct timespec MIN_PERIOD= {.tv_sec = 0, .tv_nsec = 1040000};
const struct timespec MAX_PERIOD= {.tv_sec = 0, .tv_nsec = 21000000};


struct transport{
	char run;
	char on;

	char tick;
	
	struct timespec last_tick_time; 
	struct timespec tick_period; 

	lo_server osc_server;

	struct client_list_node * tick_client_list;
	//struct client_list_node * bpm_client_list;
};


struct client{
	lo_address addr;
	char * path;
};
 
struct client_list_node{
	struct client * client;
	struct client_list_node * next;
};


struct transport * new_transport();
void start_transport_loop(struct transport * tran);

// The basic functions of the transport
void start(struct transport * trans);
void stop(struct transport * trans);
void quit(struct transport * trans);
//int tap(struct transport * trans)
//int clear_tap(struct transport * trans)
//int set_bpm(struct transport * trans)
//int factor_bpm(struct transport * trans)
//int reg_bpm_rcvr(struct transport * trans)
//int reg_tick_rcvr(struct transport * trans)


#endif
