// Monome can send the following
// - start
// - stop
// - tap
// - clear_tap
// - reg_tick
// - reg_bpm
// - inc, dec
// - set bpm

//change
//

//void *start_monome( void *ptr )
//{
//	struct transport_params * params= (struct transport_params*) ptr; 
//
//	struct monome * mono = new_monome(params->monome_port,
//			params->transport_port, params->bridge_port );
//	//mono = new_monome(params->monome_port,
//	assert( mono != NULL );
//
//	led_stop(mono);
//	led_bpm(mono);
//
//	start_monome_loop( mono );
//}


struct monome * 
new_monome( struct monome * mono, char * monome_port, char * transport_port,
		char * bridge_port)
{

	//time_t t;
	//srand((unsigned) time(&t));

	mono->run = 1;
	//mono->show = 1;

	mono->bpm_mask[0] = 128 + 64;
	mono->bpm_mask[1] = 128 + 64;
	mono->bpm_mask[2] = 0;
	mono->bpm_mask[3] = 0;
	mono->bpm_mask[4] = 0;
	mono->bpm_mask[5] = 0;
	mono->bpm_mask[6] = 0;
	mono->bpm_mask[7] = 0;

	mono->press_count = 0;

	mono->monome_osc_server = lo_server_new( monome_port, error );
	if( !(mono->monome_osc_server) ){
		//return NULL;
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

	//lo_server_add_method(mono->monome_osc_server, NULL,NULL, monome_generic_handler,
			//mono);

	//return mono;
}


void * start_monome_loop(struct monome * mono)
{
	while( mono->run ){
		lo_server_recv_noblock(mono->monome_osc_server, 50);
	}
}

void led_up(struct monome * mono)
{
	lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
	//lo_send(lo_address_new(NULL, "8000"), "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
			LED_MASK_UP);
}

void led_down(struct monome * mono)
{
	lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
	//lo_send(lo_address_new(NULL, "8000"), "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
			LED_MASK_DOWN);
}

void led_stop(struct monome * mono)
{
	lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
	//lo_send(lo_address_new(NULL, "8000"), "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
			LED_MASK_STOP);
}

void led_clear(struct monome * mono)
{
	lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
	//lo_send(lo_address_new(NULL, "8000"), "/transport/grid/led/map", "iiiiiiiiii", 0, 0,
			LED_MASK_CLEAR);
}

void led_bpm(struct monome * mono)
{
	lo_send(mono->bridge_address, "/transport/grid/led/map", "iiiiiiiiii", 8, 0,
	//lo_send(lo_address_new(NULL, "8000"), "/transport/grid/led/map", "iiiiiiiiii", 8, 0,
		mono->bpm_mask[0], mono->bpm_mask[1],
		mono->bpm_mask[2], mono->bpm_mask[3],
		mono->bpm_mask[4], mono->bpm_mask[5],
		mono->bpm_mask[6], mono->bpm_mask[7]);
}



int monome_generic_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
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


void set_bpm_mask(struct monome * mono, float bpm)
{
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
}

int monome_quit_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	mono->run = 0;
	led_clear(mono);
	return 0;
}

int monome_stop_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	led_stop(mono);
	return 0;
}

void show( struct monome * mono)
{
	//mono->show = 1;

	led_stop( mono );
	led_bpm( mono );
}

	
int show_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	show( mono );

	return 0;
}

int hide_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	//mono->show = 0;
	return 0;
}

int set_monome_bpm_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;


	float bpm = argv[0]->f;
	if( (bpm > 19) && (bpm < 401) ){
		set_bpm_mask(mono, bpm);
		//if( mono->show )
			led_bpm( mono );
	}

	return 0;
}

	int tick_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	if(argv[0]->i == 0)
		led_down(mono);
	else if(argv[0]->i == 72)
		led_up(mono);

	return 0;
}


void control_press(struct monome * mono, int x, int y){
	switch( y ){

		case 0:
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
				case 1:
					lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					break;
				case 2:
					lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					lo_send(mono->transport_address, "/transport/dec_bpm", NULL);
					break;
				case 5:
					lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					break;
				case 6:
					lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					lo_send(mono->transport_address, "/transport/inc_bpm", NULL);
					break;
			}
			break;
	}
}

void bpm_press( struct monome * mono, int x, int y ){

	//hide button
	//send hide message to the bridge! /transport/hide
}

int press_handler(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	struct monome * mono = (struct monome *) user_data;
	int x = argv[0]->i;
	int y = argv[1]->i;
	int press = argv[2]->i;

	if( press ){
		//if( mono->press_count == 1 ){
		if( x < 8 )
			control_press( mono, x, y );
			
		else
			bpm_press( mono, x, y );
		//}
	}

	else{
		if( (y > 0) && (y < 5) )
			mono->press_count = mono->press_count - 1;

		if( mono->press_count < 0 )
			mono->press_count = 0;
	}

	return 0;
}

