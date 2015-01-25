#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <regex.h>


#include "lcfg_static.h"
#include "lcfg_static.c"

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
	struct transport * trans = (struct transport *) user_data;

	regex_t regex;
	regex_t tick_client_data;
	regex_t tick_client_key;
	//regex_t bpm_client_key_regex;

	int ret;
	char msgbuf[100];

	ret = regcomp(&tick_client_key, 
			"tick-clients\*.[0-9]+", REG_NOSUB | REG_EXTENDED);
	assert(ret == 0);

	ret = regcomp(&tick_client_data, 
			"[0-9]{4,6}:(/[A-Za-z0-9_]+)+", REG_NOSUB | REG_EXTENDED);
	assert(ret == 0);



	/* Execute regular expression */
	//reti = regexec(&regex, "abc", 0, NULL, 0);
	//if (!reti) {
	//	    puts("Match");
	//}
	//else if (reti == REG_NOMATCH) {
	//	    puts("No match");
	//}
	//else {
	//	regerror(reti, &regex, msgbuf, sizeof(msgbuf));
	//	fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	//}

	if( regexec(&tick_client_key, key, 0, NULL, 0) == 0 ){
				puts(key);
	}

	else if( strcmp(key, "transport-port") == 0 ){
		puts(key);

	}
	else{

	}

	//int i;
	//char c;
	//for( i = 0; i < len; i++ ) {
	//	c = *((const char *)(data + i));
	//	printf("%c", isprint(c) ? c : '.');
	//}

	return lcfg_status_ok;
}


int main(void)
{
	// Read Args
	

	struct transport * trans;
	trans = new_transport();


	struct lcfg *c = lcfg_new("/home/dylan/.config/transport/transport.cfg");

	if( lcfg_parse(c) != lcfg_status_ok ) 
		printf("Error reading config file: %s\n", lcfg_error_get(c));

	else 
		lcfg_accept(c, config_iterator, trans);

	lcfg_delete(c);


	// forkGui
	// forkMonome


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


	
	trans->tick_client_list = NULL; 
	char * port = "57120";
	char * prefix = "/sc/transport/tick";
	add_client(&(trans->tick_client_list), &port, &prefix);
	print_client_list(trans->tick_client_list);


	return trans;
}


//
// Client operations
//

void print_client_list(struct client_list_node * node)
{
	if( node != NULL ){
		puts(node->client->prefix);
		print_client_list(node->next);
	}
	//else
		//puts("reached the null node");
}


void add_client(struct client_list_node ** node, char ** port, char ** prefix)
{
	if( *node == NULL ){
		*node = (struct client_list_node*) malloc(sizeof(struct client_list_node));

		struct client * c = (struct client*) malloc(sizeof(struct client));

		c->addr = lo_address_new("localhost", *port);
		c->prefix = *prefix;

		(*node)->next = NULL;
		(*node)->client = c;
	}
	
	else
		add_client(&((*node)->next), port, prefix);
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
