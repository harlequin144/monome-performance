#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
//#include <regex.h>


#include "lcfg_static.h"
#include "lcfg_static.c"
#include "slre.h"
#include "slre.c"

#include "time.c"
#include "transport.h"
#include "transport-osc.h"
#include "transport-osc.c"
#include "gui.c"
 

// Notes
// - Period refers to the time between ticks
// - bpm is just regular bpm
//
//
// to do
// - make sure the timing is robust before moving on!!!



enum lcfg_status
config_iterator(const char *key, void *data, size_t len, void *user_data) 
{
	struct transport_params * params = (struct transport_params *) user_data;

	if( slre_match("transport-port", key, strlen(key), NULL, 0, 0) > 0){

		const char * port = (const char *) data;
		if( slre_match("\\d\\d\\d\\d\\d?\\d?", port, strlen(key), NULL, 0, 0) > 0)
			//puts(port);
			params->transport_port = malloc(sizeof(char) * strlen(port));
			strcpy(params->transport_port, port);
	}


	if( slre_match("tick-clients\\.\\d", key, strlen(key), NULL, 0, 0) > 0){

		const char * client = (const char *) data;
		//struct slre_cap caps[4];
	

		//if(slre_match("(\\d\\d\\d\\d\\d?\\d?):\\(\\(/[A-Za-z0-9_]+\\)+/\\)", 
		//if(slre_match("((\\d\\d\\d\\d\\d?\\d?)?:([\\d\\S_]))", 
		//if( slre_match("\\(57210\\):\\(\\S+\\)", 
					//client, strlen(client), caps, 4, 0 ) > 0){

			//puts(client);
			//puts(caps[0].ptr);
			//puts(caps[1].ptr);
			////puts(caps[2].ptr);
			//puts(caps[4].ptr);

			char * regex = "(57210):(/\\S+/)";
			
			struct slre_cap caps[2];
			
			if( slre_match(regex, client, strlen(client), caps, 2, 0) > 0) {
				printf("Found URL: [%.*s]\n", caps[0].len, caps[0].ptr);
				printf("Found URL: [%.*s]\n", caps[1].len, caps[1].ptr);
			}

		//}
		
	}

	//else{
	//}

	return lcfg_status_ok;
}

void read_config(struct transport_params * params)
{
	struct lcfg *c = lcfg_new("/home/dylan/.config/transport/transport.cfg");

	if( lcfg_parse(c) != lcfg_status_ok ) 
		printf("Error reading config file: %s\n", lcfg_error_get(c));

	else 
		lcfg_accept(c, config_iterator, params);

	lcfg_delete(c);
}


int main(void)
{
	struct transport_params params;
	params.transport_port = "8001";
	params.tick_client_list = NULL;


	read_config( &params );
	//read_args( params );


	const char * port = "57120";
	char * prefix = "/sc/transport/tick";
	add_client(&(params.tick_client_list), &port, &prefix);
	

	// forkGui
	// forkMonome
	

	struct transport * trans;
	trans = new_transport( &params );
	//print_client_list( trans->tick_client_list );

	//start_x_gui();
	start_transport_loop(trans);

	// Clean up
	printf("Successf\n");
	exit(EXIT_SUCCESS);
}


void start_transport_loop(struct transport * trans)
{
	int return_code;
	struct timespec elapsed_time;
	struct timespec current_time;
	struct timespec rem = {.tv_sec = 0, .tv_nsec = 10000};
	const struct timespec sleeptime = {.tv_sec = 0, .tv_nsec = 100};

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

		lo_server_recv_noblock(trans->osc_server, 0);
		nanosleep(&sleeptime, &rem);
	}
}


struct transport * new_transport( struct transport_params * params )
{
	struct transport * trans;
	trans = (struct transport*) malloc( sizeof(struct transport));
	trans->run = 1;
	trans->on = 1;
	trans->tick = 0;


	int return_code = 0;
	return_code = clock_gettime(CLOCK_MONOTONIC, &(trans->last_tick_time));
	assert(return_code == 0);

	trans->tick_period = bpm_to_period(120);


	trans->osc_server = lo_server_new(params->transport_port, error);
	if(!(trans->osc_server)){
		puts("Exiting. failed to make ther osc server");
		exit(1);
	}

	lo_server_add_method(trans->osc_server, "/transport/quit",  NULL,
			quit_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/start", NULL,
			start_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/stop", NULL, 
			stop_handler, trans);
	//lo_server_add_method(trans->osc_server, "/transport/tap", NULL,
			//tap_handler, trans);
	//lo_server_add_method(trans->osc_server, "/transport/clear_tap",
			//clear_tap_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/set_bpm", "f",
			set_bpm_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/inc_bpm", NULL,
			inc_bpm_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/dec_bpm", NULL,
			dec_bpm_handler, trans);
	//lo_server_add_method(trans->osc_server,"/transport/factor_bpm_handler",
			//"f", factor_period, trans);
	//lo_server_add_method(trans->osc_server,"/transport/reg_bpm_rcvr_handler",
			//"is", reg_bpm_rcvr, trans);
	//lo_server_add_method(trans->osc_server,"/transport/reg_tick_rcvr_handler",
			//"is", reg_tick_rcvr, trans);
			
	lo_server_add_method(trans->osc_server,NULL, NULL, generic_handler, NULL);


	//lo_address bridge_send;
	//bridge_send = lo_address_new(NULL, "8000");
	
	trans->tick_client_list = params->tick_client_list;

	return trans;
}


//
// Client operations
//

void print_client_list(struct client_list_node * node)
{
	if( node != NULL ){
		printf("\t");
		printf(lo_address_get_port(node->client->addr));
		printf(":");
		printf(node->client->prefix);
		printf("\n");
		print_client_list(node->next);
	}
}


void 
add_client(struct client_list_node ** node, const char ** port, char ** prefix)
{
	if( *node == NULL ){
		*node = (struct client_list_node*) malloc(sizeof(struct client_list_node));

		struct client * c = (struct client*) malloc(sizeof(struct client));

		c->addr = lo_address_new("localhost", *port);
		c->prefix = *prefix;

		(*node)->next = NULL;
		(*node)->client = c;
	}
	
	else{
		assert((*node)->client != NULL);
		if( ( strcmp((*node)->client->prefix, *prefix) != 0 ) &&
				(	strcmp(lo_address_get_port( (*node)->client->addr ), *port) != 0 ) 
		){
			add_client(&((*node)->next), port, prefix);
		}
	}
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

int set_bpm(struct transport * trans, double bpm)
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
