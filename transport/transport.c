#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include <lo/lo.h>

#include "lcfg_static.h"
#include "lcfg_static.c"
#include "slre.h"
#include "slre.c"

//#include "time.c"
#include "transport.h"
#include "monome.h"
#include "monome.c"
#include "test.c"
 
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



int main( void )
{
	run_tests(); 

	struct transport_params params;
	struct transport trans;

	parse_config( &params );
	//parse_args( params );


	if( sched_setscheduler(0, SCHED_RR, &SCHED_PARAM) == SCHED_RR )
		puts("error setting the scheduler policy"); 
	

	new_transport( &trans, &params );

	print_client_list( &(trans.tick_clients));

	pthread_t monome_thread;
	if( pthread_create( &monome_thread, NULL, start_monome, (void*) &params) )
		puts("error making monome thread");
		

	start_transport_loop( &trans );


	pthread_join( monome_thread, NULL );
	exit(EXIT_SUCCESS);
}


//
// Initialization and Startup
//

void start_transport_loop( struct transport * trans )
{
	int return_code;
	struct timespec elapsed_time;
	struct timespec current_time;
	struct timespec rem = {.tv_sec = 0, .tv_nsec = 10000};
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 500000};

	while( trans->run ){
		if( trans->on ){
			return_code = clock_gettime( CLOCK_MONOTONIC, &current_time );
			assert(return_code == 0);

			elapsed_time = timespec_norm(current_time, trans->last_tick_time);

			//if( check_tick_expired(elapsed_time, trans->tick_period) ){
			if( timespec_geq( elapsed_time, trans->tick_period ) ||
					timespec_leq( timespec_norm(elapsed_time, trans->tick_period), TOL ))
			{
				trans->last_tick_time = current_time;
				send_tick_msgs(trans);
				trans->tick = ((trans->tick + 1) % TICKS_PER_BEAT);
			}
			//printf("bpm: %f\n", period_to_bpm(trans->tick_period));
		}

		lo_server_recv_noblock(trans->osc_server, 0);
		nanosleep( &sleep_time, &rem);
	}
}

//int check_tick_expired(struct timespec elapsed, struct timespec period)
//{
//	if( timespec_geq(elapsed, period) ){
//		//puts("late");
//		return 1; 
//	}
//
//	else{
//		struct timespec delta;
//		delta = timespec_norm(elapsed, period);
//		if( timespec_leq(delta, TOL) ){
//			//puts("early");
//			return 1; 
//		}
//	}
//
//	return 0;
//}


void new_transport ( 
	struct transport * trans, struct transport_params * params ) 
{
	trans->run = 1;
	trans->on = 0;
	trans->tick = 0;
	trans->tick_period = bpm_to_period(120);

	trans->tick_clients = NULL;
	trans->bpm_clients = NULL;
	trans->tap_times = NULL;

	if( clock_gettime(CLOCK_MONOTONIC, &(trans->last_tick_time)) != 0 )
		puts("failed to initialize the last tick time in new_transport");


	while( params->tick_clients != NULL ){
		char * port = malloc(sizeof(char) *
					(strlen(lo_address_get_port(params->tick_clients->addr)) + 1));
		strcpy(port, lo_address_get_port(params->tick_clients->addr));

		add_tick_client( trans, port, params->tick_clients->prefix );

		params->tick_clients = params->tick_clients->next; 
	}

	while( params->bpm_clients != NULL ){
		char * port = malloc(sizeof(char) *
					(strlen(lo_address_get_port(params->bpm_clients->addr)) + 1));
		strcpy(port, lo_address_get_port(params->bpm_clients->addr));

		add_bpm_client( trans, port, params->bpm_clients->prefix );

		params->bpm_clients = params->bpm_clients->next; 
	}


	trans->osc_server = lo_server_new(params->transport_port, error);
	if( !(trans->osc_server) )
		puts("Failed to make ther osc server");


	trans->monome_address = lo_address_new(NULL, params->monome_port);

	lo_server_add_method(trans->osc_server, "/transport/quit",  NULL,
			quit_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/start", NULL,
			start_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/stop", NULL, 
			stop_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/toggle", NULL, 
			toggle_handler, trans);

	lo_server_add_method(trans->osc_server, "/transport/tap", NULL,
			tap_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/clear_tap", NULL,
			clear_tap_handler, trans);
			
	lo_server_add_method(trans->osc_server, "/transport/set_bpm", "f",
			set_bpm_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/inc_bpm", "i",
			inc_bpm_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/dec_bpm", "i",
			dec_bpm_handler, trans);

	lo_server_add_method(trans->osc_server, "/transport/add_bpm_client",
			"ss", add_bpm_client_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/add_tick_client",
			"ss", add_tick_client_handler, trans);


	lo_server_add_method(trans->osc_server, "/transport/grid/key", "iii",
			forward_press_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/hide", NULL,
			forward_hide_handler, trans);
	lo_server_add_method(trans->osc_server, "/transport/show", NULL,
			forward_show_handler, trans);
			

	lo_server_add_method(trans->osc_server,NULL, NULL, generic_handler, trans);
}



//
// Config File Processing
//

void parse_config( struct transport_params * params )
{
	strcpy(params->transport_port, "8001");
	strcpy(params->monome_port, "8002");
	strcpy(params->bridge_port, "8000");
	params->tick_clients = NULL;
	params->bpm_clients = NULL;
	
	struct lcfg *c = lcfg_new("/home/dylan/.config/transport/transport.cfg");

	if( lcfg_parse(c) != lcfg_status_ok ) 
		printf("Error reading config file: %s\n", lcfg_error_get(c));

	else 
		lcfg_accept(c, config_iterator, params);

	lcfg_delete(c);
}


enum lcfg_status config_iterator (
	const char *key, void *data, size_t len, void *user_data 
) {
	// There may be some redundant validation happening here, but the job of
	// this function is to validate the config file and tell the user when there
	// is a problem with it. Validation of the parameters will happen when they
	// are used in one of the methods that directly alters the transport.
	
	struct transport_params * params = (struct transport_params *) user_data;
	const char * info = (const char *) data;

	if( strcmp("transport-port", key) == 0){
		if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, strlen(info)+1,NULL,0,0) > 0 )
			strncpy(params->transport_port, data, len);
		
		else{
			puts("invalid data format in config file:");
			puts(key); puts(data);
		}
	}

	else if( strcmp("bridge-port", key) == 0){
		if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, strlen(info)+1,NULL,0,0) > 0 )
			strncpy(params->bridge_port, data, len);
		
		else{
			puts("invalid data format in config file:"); puts(key); puts(data);
		}
	}

	else if( strcmp("monome-port", key) == 0){
		if( slre_match("\\d\\d\\d\\d\\d?\\d?", info, strlen(info)+1,NULL,0,0) > 0 )
			strncpy(params->monome_port, data, len);
		
		else{
			puts("invalid data format in config file:"); puts(key); puts(data);
		}
	}

	else{
		struct slre_cap caps[2];
		const char * regex = "(\\d\\d\\d\\d\\d?\\d?):(/[^\\s\\n\\r\\f\\v\\t\\b]+)";

		if( slre_match("tick-clients\\.\\d", key, strlen(key)+1, NULL, 0, 0) > 0){
			if( slre_match(regex, info, strlen(info)+1, caps, 2, 0) > 0 ){
				char * port = malloc(sizeof(char) * (caps[0].len+1));
				char * prefix = malloc(sizeof(char) * (caps[1].len+1));
				strncpy(port, caps[0].ptr, caps[0].len);
				strncpy(prefix, caps[1].ptr, caps[1].len);

				//add_client( &(params->tick_clients), port, prefix);
				add_tick_client_param( params, port, prefix);
			}
			else{
				puts("invalid data format in config file:");
				puts(key); puts(data);
			}
		}

		else 
		if( slre_match("bpm-clients\\.\\d", key, strlen(key)+1, NULL, 0, 0) > 0){
			if( slre_match(regex, info, strlen(info)+1, caps, 2, 0) > 0 ){
				char * port = malloc(sizeof(char) * (caps[0].len+1));
				char * prefix = malloc(sizeof(char) * (caps[1].len+1));
				strncpy(port, caps[0].ptr, caps[0].len);
				strncpy(prefix, caps[1].ptr, caps[1].len);

				//add_client( &(params->bpm_clients), port, prefix);
				add_bpm_client_param( params, port, prefix);
			}
			else{
				puts("invalid data format in config file:"); puts(key); puts(data);
			}
		}

		else{
			puts("Found and invalid key in config file:"); puts(key); puts(data);
		}
	}

	return lcfg_status_ok;
}



//
// Primary Transport Methods
// these are the basic functions that directly touch the transport loop
// all validation happens here.
//

void quit( struct transport * trans )
{
	trans->run = 0;
}


void set_loop_on( struct transport * trans )
{
	trans->on = 1;
}

void set_loop_off( struct transport * trans )
{
	trans->on = 0;
	send_stop_msgs(trans);
	clear_tap( trans );
	//trans->tick = 0; ???
}

void set_tick_period( struct transport * trans, struct timespec period )
{
	if( timespec_geq(period, MAX_PERIOD) )
		trans->tick_period = MAX_PERIOD;

	else if( timespec_leq(period, MIN_PERIOD) )
		trans->tick_period = MIN_PERIOD;

	else
		trans->tick_period = period;

	send_bpm_msgs( trans );
}

void add_client( 
	struct client_list_node ** tracer, char * port, char * prefix ) 
{
	// validate!!!
	if( *tracer == NULL ){
		*tracer = malloc(sizeof(struct client_list_node));

		(*tracer)->next = NULL;
		(*tracer)->prefix = malloc(sizeof(char) * (strlen(prefix) + 1)) ;
		strcpy( (*tracer)->prefix, prefix );
		(*tracer)->addr = lo_address_new(NULL, port);
	}

	else
		add_client(&(*tracer)->next, port, prefix);
}

void add_tick_client( struct transport * trans, char * port, char * prefix )
{
	add_client( &(trans->tick_clients), port, prefix);
}

void add_bpm_client( struct transport * trans, char * port, char * prefix )
{
	add_client( &(trans->bpm_clients), port, prefix );
	send_bpm_msgs( trans );
}

void add_tick_client_param( 
		struct transport_params * params, char * port, char * prefix )
{
	add_client( &(params->tick_clients), port, prefix);
}

void add_bpm_client_param( 
		struct transport_params * params, char * port, char * prefix )
{
	add_client( &(params->bpm_clients), port, prefix );
}

void push_tap_time( struct tap_list_node ** node, struct timespec tap_time )
{
	if( (*node) == NULL ){
		*node = malloc(sizeof(struct tap_list_node));

		(*node)->next = NULL;
		(*node)->time.tv_sec = tap_time.tv_sec; 
		(*node)->time.tv_nsec = tap_time.tv_nsec; 
	}

	else{
		struct tap_list_node * new = malloc(sizeof(struct tap_list_node));
		new->time.tv_sec = tap_time.tv_sec; 
		new->time.tv_nsec = tap_time.tv_nsec; 
		new->next = *node;
		*node = new;
	}
}



void calculate_tick_period( struct transport * trans )
{
	if( trans->tap_times != NULL ){
		struct tap_list_node * node = trans->tap_times;
		node = node->next; // ignore the head, it holds the most recent tap time
		

		if( node != NULL){
			//struct timespec sum;
			//sum.tv_sec = 0;
			//sum.tv_nsec = 0;
			double sum = 0.0;
			int num_taps = 0;
			
			while( node != NULL ){
				puts("adding time spec to sum:");
				print_timespec( node->time );

				sum += timespec_to_double(node->time);
				//sum.tv_sec += node->time.tv_sec;
				//sum.tv_nsec += node->time.tv_nsec;

				num_taps = num_taps + 1;
				node = node->next;
			}
			
			printf("sum calculatd:%0.10f\n", sum);
			//print_timespec( sum );

			// heres out problem...
			//sum.tv_sec = sum.tv_sec / (num_taps * TICKS_PER_BEAT);
			//sum.tv_nsec = (sum.tv_nsec / (num_taps * TICKS_PER_BEAT));
			 
			sum /= num_taps;
			sum /= TICKS_PER_BEAT;

			printf("num taps: %d\n", num_taps);
			set_tick_period( trans, double_to_timespec(sum) );
			printf("bpm: %f\n", period_to_bpm(trans->tick_period));
		}
	}
}

void tap( struct transport * trans, struct timespec tap_time )
{ 
	if( trans->tap_times != NULL ){
		struct timespec delta = timespec_norm( trans->tap_times->time, tap_time );

		if( timespec_geq( delta, TAP_TIMES_EXPIRE ) ){
			clear_tap( trans );
			puts("Expiration");
			push_tap_time( &(trans->tap_times), tap_time );
		}

		else 
		//if( timespec_geq(delta, MIN_BEAT_PERIOD) && 
				//timespec_geq(MAX_BEAT_PERIOD, delta) 
		//)
		{
			//puts("normal push");
			trans->tap_times->time = delta;
			push_tap_time( &(trans->tap_times), tap_time );
		}
		//else
			//puts("not expire, not right speed");

		calculate_tick_period( trans );
	}

	else{
		push_tap_time( &(trans->tap_times), tap_time );
		puts("initial push");
	}

	print_tap_times(trans->tap_times);

	trans->tick = 0;
	set_loop_on( trans );
}


void clear_tap( struct transport * trans )
{
	if( trans->tap_times != NULL ){
		struct tap_list_node * node = trans->tap_times;

		trans->tap_times = trans->tap_times->next;
		free( node );
		clear_tap( trans );
	}
}

//
// Secondary Transport Methods
//

void set_bpm( struct transport * trans, double bpm )
{
	set_tick_period( trans, bpm_to_period(bpm) );
}


void print_client_list( struct client_list_node ** list )
{

	if( *list != NULL ){
		printf("%s:", lo_address_get_port((*list)->addr));
		printf("%s\n", (*list)->prefix);

		print_client_list(&((*list)->next));
	}
	else{
		puts("End of client list");
	}
}

void print_tap_times( struct tap_list_node * node )
{
	if( node != NULL ){
		printf("secs: %li, nsecs %li\n", 
				node->time.tv_sec, node->time.tv_nsec );
		print_tap_times( node->next );
	}
}

void print_timespec(struct timespec time)
{
	printf("\t %li \n\t %li\n", time.tv_sec, time.tv_nsec);
}


//
// OSC
//

int generic_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
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


int quit_handler ( 
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/quit", NULL);
	quit(trans);
	return 0;
}

int toggle_handler ( 
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	if( trans->on )
		set_loop_off( trans );
	else
		set_loop_on( trans );

	return 0;
}

int start_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	set_loop_on( trans );
	return 0;
}

int stop_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	set_loop_off( trans );
	send_stop_msgs( trans );
	return 0;
}

int tap_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	struct timespec time;
	clock_gettime( CLOCK_MONOTONIC, &time );

	tap( trans, time );
}

int clear_tap_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct transport * trans = (struct transport *) user_data;
	clear_tap( trans );
	puts("clear");

}

int set_bpm_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data )
{
	struct transport * trans = (struct transport *) user_data;
	set_bpm(trans, argv[0]->f) ;
}

int inc_bpm_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data )
{
	struct transport * trans = (struct transport *) user_data;
	set_bpm(trans, (period_to_bpm(trans->tick_period) + ((double) argv[0]->i)));
}

int dec_bpm_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data ) 
{
	struct transport * trans = (struct transport *) user_data;
	set_bpm(trans, period_to_bpm(trans->tick_period) - ((double) argv[0]->i));
}

int add_bpm_client_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data ) 
{
	struct transport * trans = (struct transport *) user_data;
	add_bpm_client( trans, &(argv[1]->s), &(argv[0]->s));
}

int add_tick_client_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data )
{
	struct transport * trans = (struct transport *) user_data;
	//char * port = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 4));
	//char * prefix = malloc(sizeof(char) * (strlen(&(argv[1]->S)) + 4));
	//strcpy(port, &(argv[1]->s));
	//strcpy(prefix, &(argv[0]->s));
	add_tick_client( trans, &(argv[1]->s), &(argv[0]->s));
}


int forward_press_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;

	lo_send(trans->monome_address, "/transport/monome/grid/key", "iii",
			argv[0]->i, argv[1]->i, argv[2]->i);
}

int forward_show_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/show", NULL);
}

int forward_hide_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct transport * trans = (struct transport *) user_data;
	lo_send(trans->monome_address, "/transport/monome/hide", NULL);
}




// osc server error func
void error( int num, const char *msg, const char *path )
{
	printf("loblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


//
// Outgoing OSC 
//
//


void send_tick_msgs( struct transport * trans )
{
	struct client_list_node * iterator = trans->tick_clients;
	char * path;
	lo_message message = lo_message_new();

	lo_message_add_int32( message, trans->tick );

	while(iterator != NULL){
		path = malloc(sizeof(char) * (strlen(iterator->prefix) + 6) ); 
		strcpy( path, iterator->prefix );
		strcat( path, "/tick" );
		
		lo_send_message(iterator->addr, path, message);

		iterator = iterator->next;
		free(path);
	}

	lo_message_free( message );
}

void send_bpm_msgs( struct transport * trans )
{
	struct client_list_node * iterator = trans->tick_clients;
	char * path;
	lo_message message = lo_message_new();
	
	lo_message_add_float( message, period_to_bpm(trans->tick_period) );

	while(iterator != NULL){
		path = malloc(sizeof(char) * (strlen(iterator->prefix) + 5) ); 
		strcpy( path, iterator->prefix );
		strcat( path, "/bpm" );
		
		lo_send_message(iterator->addr, path, message);

		iterator = iterator->next;
		free(path);
	}

	lo_message_free( message );
}

void send_stop_msgs( struct transport * trans )
{
	struct client_list_node * iterator = trans->tick_clients ;
	char * path;
	lo_message message = lo_message_new();
	
	lo_message_add_nil( message );

	while(iterator != NULL){
		path = malloc(sizeof(char) * (strlen(iterator->prefix) + 6) ); 
		strcpy( path, iterator->prefix );
		strcat( path, "/stop" );
		
		lo_send_message(iterator->addr, path, message);
		
		iterator = iterator->next;
		free(path);
	}

	lo_message_free( message );
}







//
// Time Functions
//

//struct timespec timespec_max(struct timespec time1, struct timespec time2)
//{
//	if( timespec_geq(time1, time2) )
//		return time1;
//	else 
//		return time2;
//}
//
//struct timespec timespec_min(struct timespec time1, struct timespec time2)
//{
//	if( timespec_leq(time1, time2) )
//		return time1;
//	else 
//		return time2;
//}

int timespec_geq(struct timespec time1, struct timespec time2)
{
	if( difftime(time1.tv_sec,time2.tv_sec) > 0 )
		return 1;
	
	else 
	if( (time1.tv_sec == time2.tv_sec) && (time1.tv_nsec >= time2.tv_nsec) )
		return 1;

	else
		return 0;
}

int timespec_leq(struct timespec time1, struct timespec time2)
{
	if( difftime(time1.tv_sec,time2.tv_sec) < 0 )
		return 1;
	
	else if( 
		//( abs(difftime(time1.tv_sec,time2.tv_sec)) < 0.0000000001 ) &&	
		( time1.tv_sec == time2.tv_sec) && ( time1.tv_nsec <= time2.tv_nsec ) 
	)
		return 1;

	else
		return 0;
}



struct timespec timespec_norm(struct timespec time1, struct timespec time2)
{
	struct  timespec  result;
	result.tv_nsec = result.tv_nsec = 0 ;

	if(time1.tv_sec == time2.tv_sec){
		result.tv_sec = 0;
		result.tv_nsec = abs((time1.tv_nsec - time2.tv_nsec));
	}
	else
		
	if(time1.tv_sec < time2.tv_sec){		
		result.tv_nsec = NANOS_PER_SEC - time1.tv_nsec;
		time1.tv_sec += 1;

		result.tv_nsec += time2.tv_nsec;

		result.tv_sec = abs(time1.tv_sec - time2.tv_sec);
		result.tv_sec += result.tv_nsec / NANOS_PER_SEC;
		result.tv_nsec = result.tv_nsec % NANOS_PER_SEC;
	} 
	else {						
		result =  timespec_norm(time2, time1);
	}
	
	return result;
	//return double_to_timespec( 
			//fabs( timespec_to_double(time1) - timespec_to_double(time2)) );
}

double period_to_bpm(struct timespec period)
{
	double sec_per_tick = (double) period.tv_sec;
	sec_per_tick += (((double)period.tv_nsec) /((double)  NANOS_PER_SEC));

	double tick_per_sec = ((double) 1.0) / sec_per_tick;
	double beat_per_sec = tick_per_sec / TICKS_PER_BEAT;
	double bpm = beat_per_sec * 60;

	return bpm;
}
	
struct timespec bpm_to_period(double bpm)
{
	double sec_per_tick = ((double)1.0 / ((bpm * TICKS_PER_BEAT) / 60.0));
	long long sec = (long long) sec_per_tick;
	long long nsec = ((long long)((sec_per_tick - sec) * NANOS_PER_SEC));

	struct timespec ret;
	ret.tv_sec = sec;
	ret.tv_nsec = nsec;

	return ret;
}

struct timespec double_to_timespec(double time)
{
	long long sec = (long long) time;
	long long nsec = ((long long)((time - sec) * NANOS_PER_SEC));

	struct timespec ret;
	ret.tv_sec = abs(sec);
	ret.tv_nsec = abs(nsec);

	return ret;
}

double timespec_to_double(struct timespec time)
{
	double ret = 0.0;
	
	ret += abs(time.tv_sec);
	//printf("%0.10f\n", ret);
	//ret += abs((((double) time.tv_nsec) / ((double) NANOS_PER_SEC)));
	ret += (((double) time.tv_nsec) / 1000000000.0);
	//printf("%0.10f\n", ret);

	return ret;
}

//int validate_bpm(double bpm)
//{
//	return validate_period( bpm_to_period(bpm) );
//}
//
//
//int validate_period(struct timespec period)
//{
//	if( timespec_geq(period, MIN_PERIOD) && timespec_leq(period, MAX_PERIOD) )
//		return 1;
//	else
//		return 0;
//}
