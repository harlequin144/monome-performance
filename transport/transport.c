#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


#include "lcfg_static.h"
#include "lcfg_static.c"
#include "slre.h"
#include "slre.c"

#include <lo/lo.h>

struct monome{
	int run;
	int show; 
	int press_count;
	int bpm_mask[8];

	lo_server monome_osc_server;
	lo_address bridge_address;
	lo_address transport_address;

};



#include "time.c"
#include "transport.h"
//#include "gui.c"
#include "monome.h"
#include "monome.c"
 
#define GENERIC_PRINT 0

// Notes
// - Period refers to the time between ticks
// - bpm is just regular bpm
//
// - Consider switching to tre for the regex engine.
//
// to do
// - make sure the timing is robust before moving on!!!

// Think about varying the sleep times.!!!



int main(void)
{
	struct transport_params params;
	params.transport_port = "8001";
	params.monome_port = "8002";
	params.bridge_port = "8000";
	params.tick_client_list = NULL;
	params.bpm_client_list = NULL;


	//parse_config( &params );
	//parse_args( params );
	


	//pthread_t monome_thread;
	//if( pthread_create( &monome_thread, NULL, start_monome, (void*) &params) )
		//puts("error making monome thread");


	if( sched_setscheduler(0, SCHED_RR, &SCHED_PARAM) == SCHED_RR )
		puts("error setting the scheduler policy");


	struct transport * trans = new_transport( &params );
	assert( trans != NULL );

	struct monome * mono = new_monome("8002", "8001", "8000" );

	//add_client(&(trans->tick_client_list), "8002", "/transport/monome");
	//add_client(&(trans->bpm_client_list), "8002", "/transport/monome");

	send_bpm_msgs( trans );
	
	//print_client_list(trans->tick_client_list);


	start_transport_loop( trans, mono);

	//start_x_gui();


	//pthread_join( monome_thread, NULL );
	exit(EXIT_SUCCESS);
}


//void start_transport_loop(struct transport * trans)
void start_transport_loop(struct transport * trans, struct monome * mono)
{
	int return_code;
	struct timespec elapsed_time;
	struct timespec current_time;
	struct timespec rem = {.tv_sec = 0, .tv_nsec = 10000};
	//const struct timespec sleeptime = {.tv_sec = 0, .tv_nsec = 500000};

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
		lo_server_recv_noblock(mono->monome_osc_server, 0);
		nanosleep(&SLEEP_TIME, &rem);
	}
}


struct transport * new_transport( struct transport_params * params )
{
	struct transport * trans;
	trans = (struct transport*) malloc( sizeof(struct transport));

	trans->run = 1;
	trans->on = 0;
	trans->tick = 0;

	trans->bpm_client_list = params->bpm_client_list;
	trans->tick_client_list = params->tick_client_list;

	int return_code = 0;
	return_code = clock_gettime(CLOCK_MONOTONIC, &(trans->last_tick_time));
	assert(return_code == 0);

	trans->tick_period = bpm_to_period(120);


	trans->osc_server = lo_server_new(params->transport_port, error);
	if(!(trans->osc_server)){
		puts("Failed to make ther osc server");
		return NULL;
	}


	trans->monome_address = lo_address_new(NULL, params->monome_port);

	lo_server_add_method(trans->osc_server, "/transport/quit",  NULL,
			quit_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/start", NULL,
			start_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/stop", NULL, 
			stop_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/toggle", NULL, 
			toggle_handler, trans);
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
	lo_server_add_method(trans->osc_server,"/transport/add_bpm_client",
			"ss", add_bpm_client_handler, trans);
	lo_server_add_method(trans->osc_server,"/transport/add_tick_client",
			"ss", add_tick_client_handler, trans);

	lo_server_add_method(trans->osc_server, "/transport/grid/key", "iii",
			monome_press_handler, trans);

	lo_server_add_method(trans->osc_server, "/transport/hide", NULL,
			hide_handler, trans);

	lo_server_add_method(trans->osc_server, "/transport/show", NULL,
			show_handler, trans);
			
	lo_server_add_method(trans->osc_server,NULL, NULL, generic_handler, trans);


	
	return trans;
}



//
// Config File Processing
//

//void parse_config(struct transport_params * params)
//{
//	struct lcfg *c = lcfg_new("/home/dylan/.config/transport/transport.cfg");
//
//	if( lcfg_parse(c) != lcfg_status_ok ) 
//		printf("Error reading config file: %s\n", lcfg_error_get(c));
//
//	else 
//		lcfg_accept(c, config_iterator, params);
//
//	lcfg_delete(c);
//}
//
//
//enum lcfg_status
//config_iterator(const char *key, void *data, size_t len, void *user_data) 
//{
//	// There may be some redundant validation happening here, but the job of
//	// this function is to validate the config file and tell the user when there
//	// is a problem with it. Validation of the parameters will happen when they
//	// are used in one of the methods that directly alters the transport.
//	
//	struct transport_params * params = (struct transport_params *) user_data;
//	const char * info = (const char *) data;
//
//	if( strcmp("transport-port", key) == 0){
//		if(slre_match("\\d\\d\\d\\d\\d?\\d?", info, strlen(key)+1, NULL, 0, 0)>0){
//
//			params->transport_port = malloc(sizeof(char) * (len+1));
//			int i;
//			for( i = 0; i < len; i++ )
//				params->transport_port[i] = *((const char *)(data + i));
//			
//			params->transport_port[len] = '\0';
//		}
//		else{
//			puts("invalid data format in config file:");
//			puts(key); puts(data);
//		}
//	}
//
//	else if( strcmp("bridge-port", key) == 0){
//		if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, len, NULL, 0, 0) > 0){
//
//			params->bridge_port = malloc(sizeof(char) * (len+1));
//			int i;
//			for( i = 0; i < len; i++ )
//				params->bridge_port[i] = *((const char *)(data + i));
//			
//			params->bridge_port[len] = '\0';
//		}
//		else{
//			puts("invalid data format in config file:");
//			puts(key); puts(data);
//		}
//	}
//
//	else if( strcmp("monome-port", key) == 0){
//		if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, len, NULL, 0, 0) > 0){
//
//			params->monome_port = malloc(sizeof(char) * (len+1));
//			int i;
//			for( i = 0; i < len; i++ )
//				params->monome_port[i] = *((const char *)(data + i));
//			
//			params->monome_port[len] = '\0';
//		}
//		else{
//			puts("invalid data format in config file:");
//			puts(key); puts(data);
//		}
//	}
//
//	//else if( strcmp("gui-port", key) == 0){
//	//	if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, strlen(key)+1,  NULL, 0, 0) > 0){
//	//		
//	//		params->gui_port = malloc(sizeof(char) * len);
//	//		strcpy(params->gui_port, info);
//	//	}
//	//	else{
//	//		puts("invalid data format in config file:");
//	//		puts(key); puts(data);
//	//	}
//	//}
//
//	else{
//		struct slre_cap caps[2];
//		const char * regex = "(\\d\\d\\d\\d\\d?\\d?):(/[^\\s\\n\\r\\f\\v\\t\\b]+)";
//
//		if( slre_match("tick-clients\\.\\d", key, strlen(key)+1, NULL, 0, 0) > 0){
//
//			if( slre_match(regex, info, strlen(info)+1, caps, 2, 0) > 0){
//				//printf("Found URL: [%.*s]\n", caps[0].len, caps[0].ptr);
//				char * port = malloc(sizeof(char) * (caps[0].len+1));
//				char * prefix = malloc(sizeof(char) * (caps[1].len+1));
//				strncpy(port, caps[0].ptr, caps[0].len+1);
//				strncpy(prefix, caps[1].ptr, caps[1].len+1);
//				port[caps[0].len] = '\0';
//				prefix[caps[1].len] = '\0';
//				add_client( &(params->tick_client_list), &port, &prefix );
//			}
//			else{
//				puts("invalid data format in config file:");
//				puts(key); puts(data);
//			}
//		}
//
//		else 
//		if( slre_match("bpm-clients\\.\\d", key, strlen(key)+1, NULL, 0, 0) > 0){
//
//			if( slre_match(regex, info, strlen(info)+1, caps, 2, 0) > 0)
//			{
//				char * port = malloc(sizeof(char) * (caps[0].len+1));
//				char * prefix = malloc(sizeof(char) * (caps[1].len+1));
//				strncpy(port, caps[0].ptr, caps[0].len+1);
//				strncpy(prefix, caps[1].ptr, caps[1].len+1);
//				port[caps[0].len] = '\0';
//				prefix[caps[1].len] = '\0';
//				add_client( &(params->bpm_client_list), &port, &prefix );
//			}
//			else{
//				puts("invalid data format in config file:");
//				puts(key); puts(data);
//			}
//		}
//
//		else{
//			puts("Found and invalid key in config file:");
//			puts(key); puts(data);
//		}
//
//	}
//
//	return lcfg_status_ok;
//}


//
// Client Methods
//

void print_client_list(struct client_list_node * node)
{
	if( node != NULL ){
		printf("\t->");
		printf(lo_address_get_port(node->client->addr));
		printf(":");
		printf(node->client->prefix);
		printf("<-\n");
		print_client_list(node->next);
	}
	else{
		puts("reached end");
	}
}
//
// Transport Methods
// these are the basic functions that directly touch the transport loop
// all validation happens here.
//

void quit(struct transport * trans)
{
	trans->run = 0;
}


void set_loop_on(struct transport * trans)
{
	trans->on = 1;
}

void set_loop_off(struct transport * trans)
{
	trans->on = 0;
	send_stop_msgs(trans);
}

int set_bpm(struct transport * trans, double bpm)
{
	struct timespec period = bpm_to_period( bpm );
	if( validate_period(period) ){
		trans->tick_period = period;
		send_bpm_msgs( trans );
	}
}

void 
//add_client(struct client_list_node ** node, char ** port, char ** prefix)
add_client(struct client_list_node ** node, char * port, char * prefix)
{
	if( *node == NULL ){
		if(
			(slre_match("\\d\\d\\d\\d\\d?\\d?", port, strlen(port) + 1, NULL,0,0)
			 > 0)
		&&
			(slre_match("/\\S+", prefix, strlen(prefix) + 1, NULL, 0, 0) > 0)
		)
		{	
			*node = 
				(struct client_list_node*) malloc(sizeof(struct client_list_node));
			(*node)->client = (struct client*) malloc(sizeof(struct client));
			(*node)->next = NULL;

			(*node)->client->prefix = malloc(sizeof(char) * (strlen(prefix) + 1));


			(*node)->client->addr = lo_address_new("localhost", port);
			memcpy((*node)->client->prefix, prefix, strlen(prefix) + 1);

		}
		else
			puts("got an attempt to register invalid bpm client");
	}
	
	else{
		assert((*node)->client != NULL);
		if( ( strcmp((*node)->client->prefix, prefix) != 0 ) ||
				(	strcmp(lo_address_get_port( (*node)->client->addr ), port) != 0 ) 
		){
			add_client(&((*node)->next), port, prefix);
		}
	}
}



//int tap(const char *path, const char *types, lo_arg ** argv,
//int clear_tap(const char *path, const char *types, lo_arg ** argv,


//
// OSC
//

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	if( GENERIC_PRINT ){
		int i;

		printf("path: <%s>\n", path);
		for (i = 0; i < argc; i++) {
			printf("arg %d '%c' ", i, types[i]);
			lo_arg_pp((lo_type)types[i], argv[i]);
			printf("\n");
		}
		printf("\n");
		fflush(stdout);
	}
	return 1;
}


int quit_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/quit", NULL);
	quit(trans);
	return 0;
}

int toggle_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	if( trans->on )
		set_loop_off( trans );
	else
		set_loop_on( trans );

	return 0;
}

int start_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	set_loop_on( trans );
	return 0;
}

int stop_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	set_loop_off( trans );
	send_stop_msgs( trans );
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
	set_bpm(trans, argv[0]->f) ;
}

int inc_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	set_bpm(trans, period_to_bpm(trans->tick_period) + 1);
}

int dec_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	set_bpm(trans, period_to_bpm(trans->tick_period) - 1);
}

int add_bpm_client_handler(const char *path, const char *types, lo_arg **
												argv, int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	//char * port = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 5));
	//char * prefix = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 5));
	//strcpy(port, &(argv[1]->s));
	//strcpy(prefix, &(argv[0]->s));
	//port[strlen(&(argv[1]->S)) + 1] = '\0';
	//prefix[strlen(&(argv[1]->S)) + 1] = '\0';
	//puts(prefix);
	//add_client( &(trans->bpm_client_list), &port, &prefix );
}

int add_tick_client_handler(const char *path, const char *types, lo_arg **
												argv, int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	//char * port = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 4));
	//char * prefix = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 4));
	//strcpy(port, &(argv[1]->s));
	//strcpy(prefix, &(argv[0]->s));
	//port[strlen(&(argv[1]->S)) + 1] = '\0';
	//prefix[strlen(&(argv[1]->S)) + 1] = '\0';
	//puts(prefix);
	
	//add_client( &(trans->tick_client_list), &port, &prefix );
	
}


int monome_press_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;

	lo_send(trans->monome_address, "/transport/monome/grid/key", "iii",
			argv[0]->i, argv[1]->i, argv[2]->i);
}

int monome_show_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/show", NULL);
}

int monome_hide_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/hide", NULL);
}

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
	//struct client_list_node * iterator = malloc(sizeof(struct client_list_node));
	//iterator = trans->tick_client_list;
	//char * path;

	//print_client_list(trans->bpm_client_list);

	
	//while(iterator != NULL){
	//	path = 
	//		malloc(sizeof(char) * (strlen(iterator->client->prefix) + 1+ 5));
	//	char * prefix = malloc(sizeof(char));
	//	prefix = iterator->client->prefix;
	//	strcpy(path, prefix);
	//	strcat(path, "/tick");
	//	lo_send(iterator->client->addr, path, "i", trans->tick);

	//	iterator = iterator->next;
	//}


	lo_address mono = lo_address_new(NULL, "8002");
	lo_send(mono,"/transport/monome/tick", "i", trans->tick);
}

void send_bpm_msgs(struct transport * trans)
{
	//printf("bpm: %f\n", period_to_bpm(trans->tick_period));
	//struct client_list_node * iterator = trans->bpm_client_list;
	//
	//while(iterator != NULL){
	//	char * path = 
	//		malloc(sizeof(char) * (strlen(iterator->client->prefix)+1 + 4));
	//	strcpy(path, iterator->client->prefix);
	//	strcat(path, "/bpm");
	//	lo_send(iterator->client->addr, path, "f",
	//			period_to_bpm(trans->tick_period));

	//	iterator = iterator->next;
	//}
	lo_address mono = lo_address_new(NULL, "8002");
	lo_send(mono,"/transport/monome/bpm", "f",
			period_to_bpm(trans->tick_period));
}

void send_stop_msgs(struct transport * trans)
{
	//struct client_list_node * iterator = trans->tick_client_list;
	//
	//while(iterator != NULL){
	//	char * path = 
	//		malloc(sizeof(char) * (strlen(iterator->client->prefix)+1+5));
	//	strcpy(path, iterator->client->prefix);
	//	strcat(path, "/stop");
	//	lo_send(iterator->client->addr, path, "i", trans->tick);

	//	iterator = iterator->next;
	//}
	lo_address mono = lo_address_new(NULL, "8002");
	lo_send(mono,"/transport/monome/stop", NULL);
}
