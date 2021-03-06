#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include <lo/lo.h>
 

// Monome can send the following
// - start
// - stop
// - tap
// - clear_tap
// - reg_tick
// - reg_bpm
// - inc, dec
// - set bpm


void *start_monome( void *ptr )
{
	//struct transport_params * params = (struct transport_params *) ptr; 
	struct monome mono; // = malloc(sizeof(struct monome));

	new_monome( &mono, "8002", "8001", "8000" );

	start_monome_loop( &mono );
}

void error___( int num, const char *msg, const char *path )
{
	//printf("liblo server error");
	//fflush(stdout);
}

struct monome * new_monome (
	struct monome * mono, char * monome_port, char * transport_port, char *
	bridge_port
) {
	time_t t;
	srand((unsigned) time(&t));

	mono->run = 1;
	mono->show = 0;
	mono->press_count = 0;


	mono->bpm_mask[0] = 128 + 64; mono->bpm_mask[1] = 128 + 64;
	mono->bpm_mask[2] = 0; mono->bpm_mask[3] = 0; mono->bpm_mask[4] = 0;
	mono->bpm_mask[5] = 0; mono->bpm_mask[6] = 0; mono->bpm_mask[7] = 0;


	mono->monome_osc_server = lo_server_new( monome_port, error___ );
	if( !(mono->monome_osc_server) ){
	}


	mono->bridge_address = lo_address_new("localhost", bridge_port);
	mono->transport_address = lo_address_new("localhost", transport_port);


	lo_send(mono->transport_address, "/transport/add_tick_client", "ss",
			"/transport/monome", monome_port);

	lo_send(mono->transport_address, "/transport/add_bpm_client", "ss",
			"/transport/monome", monome_port);


	lo_server_add_method(mono->monome_osc_server, "/transport/monome/quit",
			NULL, monome_quit_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/show", 
			NULL, show_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/hide",
			NULL, hide_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/stop",
			NULL, monome_stop_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/bpm", 
			"f", set_monome_bpm_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/tick", 
			"i", tick_handler, mono);

	lo_server_add_method(mono->monome_osc_server, "/transport/monome/grid/key",
			"iii", press_handler, mono);

	//lo_server_add_method(mono->monome_osc_server, NULL,NULL,
	//monome_generic_handler, mono);

}


void * start_monome_loop( struct monome * mono )
{
	while( mono->run ){
		lo_server_recv_noblock(mono->monome_osc_server, 50);
	}
}

//
// Led and Appearance
//

void led_up( struct monome * mono )
{
	if( mono->show )
		lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 
				0, 0, LED_MASK_UP);
}

void led_down( struct monome * mono )
{
	if( mono->show )
		lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 
				0, 0, LED_MASK_DOWN);
}

void led_stop( struct monome * mono )
{
	if( mono->show )
		lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 
				0, 0, LED_MASK_STOP);
}

void led_clear( struct monome * mono )
{
	if( mono->show )
		lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 
				0, 0, LED_MASK_CLEAR);
}

void led_bpm( struct monome * mono )
{
	if( mono->show )
		//puts("led bpm");
		//puts(mono->);
		//puts(lo_address_get_port(mono->bridge_address));

		lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 
				8, 0, 
				mono->bpm_mask[0], mono->bpm_mask[1], 
				mono->bpm_mask[2], mono->bpm_mask[3], 
				mono->bpm_mask[4], mono->bpm_mask[5],
				mono->bpm_mask[6], mono->bpm_mask[7]
		);
}

void set_bpm_mask(struct monome * mono, float bpm)
{
	if( (bpm > 19) && (bpm < 401) ){
		mono->bpm_mask[0] = 128 + 64;
		mono->bpm_mask[1] = 128 + 64;
		mono->bpm_mask[2] = 0; mono->bpm_mask[3] = 0;
		mono->bpm_mask[4] = 0; mono->bpm_mask[5] = 0;
		mono->bpm_mask[6] = 0; mono->bpm_mask[7] = 0;
	
		int i, row, r;
		int bpm_ten = (int) bpm / 10;
		int bpm_one = ((int) bpm % 10) / 2;
	
		for(i = 4; i < bpm_one + 4; i++){
			mono->bpm_mask[i] += 128;
		}
	
		i = 0; row = 0;
		while( i < bpm_ten ){
			r = rand() % 7;
			r = pow(2, r);
			if( (r & mono->bpm_mask[row]) == 0 ){
				mono->bpm_mask[row] |= r;
				i++;
			}
	
			row = (row + 1) % 8;
		}
	
		led_bpm( mono );
	}
}


void control_press( struct monome * mono, int x, int y )
{
	switch( y ){

		case 0:
			mono->press_count = mono->press_count + 1;
			if( mono->press_count == 1 )
				lo_send(mono->transport_address, "/transport/toggle", NULL);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
			mono->press_count = mono->press_count + 1;
			if( mono->press_count == 1 ){
				if( x < 4 )
					lo_send(mono->transport_address, "/transport/clear_tap", NULL);
				else
					lo_send(mono->transport_address, "/transport/tap", NULL);
			}
			break;

		case 5:
		case 6:
		case 7:
			switch( x ){
				int i;
				case 1:
					lo_send(mono->transport_address, "/transport/dec_bpm", "i", 3);
					break;
				case 2:
					//lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					lo_send(mono->transport_address, "/transport/dec_bpm", "i", 2);
					break;
				case 5:
					//lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					lo_send(mono->transport_address, "/transport/inc_bpm", "i", 2);
					break;
				case 6:
					i = 3;
					lo_send(mono->transport_address, "/transport/inc_bpm", "i", i);
					break;
			}
			break;
	}
}

void bpm_press( struct monome * mono, int x, int y ){

	//hide button
	if( (x == 14) && (y == 0) ||
			(x == 14) && (y == 1) ||
			(x == 15) && (y == 0) ||
			(x == 15) && (y == 1) )
	{
		mono->show = 0;
		mono->press_count = 0;

		led_clear( mono );
		lo_send(mono->bridge_address, "/transport/hide", NULL);
	}
}


//
// OSC Handlers
//

int monome_quit_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;
	mono->run = 0;
	led_clear(mono);
	return 0;
}

int monome_stop_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;

	led_stop(mono);

	return 0;
}

	
int show_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;

	mono->show = 1;
	mono->press_count = 0;

	led_stop( mono );
	led_bpm( mono );

	return 0;
}

int hide_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;

	mono->show = 0;
	mono->press_count = 0;

	led_clear( mono );
	lo_send(mono->bridge_address, "/transport/hide", NULL);

	return 0;
}

int set_monome_bpm_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;

	set_bpm_mask( mono, argv[0]->f );

	return 0;
}

int tick_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;

	if(argv[0]->i == 0)
		led_down( mono );
	else if(argv[0]->i == 72)
		led_up( mono );

	return 0;
}


int press_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data
) {
	struct monome * mono = (struct monome *) user_data;
	int x = argv[0]->i;
	int y = argv[1]->i;
	int press = argv[2]->i;
	//printf("%i, %i, %i \n", x,y,press);
	
	if( !mono->show ){
		mono->show = 1;
		led_stop( mono );
		led_bpm( mono );
	}

	if( press ){
		if( x < 8 )
			control_press( mono, x, y );
			
		else
			bpm_press( mono, x, y );
	}

	else{
		if( (y >= 0) && (y < 5) )
			mono->press_count = mono->press_count - 1;

		if( mono->press_count < 0 )
			mono->press_count = 0;
	}

	//printf("press count:%i\n", mono->press_count);
	return 0;
}

int monome_generic_handler (
	const char *path, const char *types, lo_arg ** argv, int argc, void *data,
	void *user_data ) 
{
	struct monome * mono = (struct monome *) user_data;
	lo_send(mono->transport_address, "/monome_generic", "s", path);
	lo_send(mono->transport_address, "/monome_generic", "s", types);
//	int i;

//	printf("path: <%s>\n", path);
//	for (i = 0; i < argc; i++) {
//		printf("arg %d '%c' ", i, types[i]);
//		lo_arg_pp((lo_type)types[i], argv[i]);
//		printf("\n");
//	}
//	printf("\n");
//	fflush(stdout);
	return 1;
}



