#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "lo/lo.h"
//#include "config.h"

#define MODE_CUTTER						0	// Main mode
#define MODE_MUTE_GROUP_EDIT	1
#define MODE_SPEED_CHANGE			2
#define MODE_MIXER						3
//#define MODE_



// GLOBAL VARIABLES

// State
char quit = 0;
char mode = MODE_CUTTER; 
char show = 0; //0 don't show, 1 show
int  hold_mask = 0;
char last_press[16];

// Ports
lo_address monome_send;
lo_address sc_send;
lo_address bridge_send;


// FUNCTION PROTOTYPES

//General Prototypes
void led_clear();

// Osc Server Prototypes
void error(int num, const char *m, const char *path);

int	 press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 hide_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 channel_led_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 generic_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

//int	 example_handler(const char *path, const char *types, lo_arg ** argv,
//                 int argc, void *data, void *user_data);


// MAIN FUNCTION

int 
main(int argc, char **argv)
{
	int i=0, j  = 0;
	// Option Parsing
	
	char* listen_port = "9999";
	char* bridge_port = "8000";
	//char* input_file;
	char option;
	opterr = 0;

	while((option = getopt(argc, argv, "l:b:")) != -1)
		switch(option){
			case 'l': //Listen
				//Check to make sure that it is a valid port here.
				listen_port = optarg;
				break;

			case 'b': //Bridge
				//Check to make sure that it is a valid port here.
				bridge_port = optarg;
				break;

			//case 'm': //monome port
			//case 's': //send port
			case '?':
				if(optopt == 'p')
	 				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      	else if (isprint (optopt))
	 		  	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      	else
	 		  	fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
      	return 1;
	 	  default:
				abort();
		}
	
  printf("listen port = %s, \n",listen_port);
  printf("bridge port = %s, \n",bridge_port);
	       
  if(i < argc)
 	   printf ("Non-option argument %s\n", argv[i]);
	


	//Initialization
	
	// OSC Server
  lo_server osc_server = lo_server_new(listen_port, error);	
	if(!osc_server){
		puts("Exiting. Failed to create server.");
		exit(1);
	}

  lo_server_add_method(osc_server, "/cutter/grid/key","iii", press_handler, NULL);
  lo_server_add_method(osc_server, "/cutter/quit", "", quit_handler, NULL);	
  lo_server_add_method(osc_server, "/cutter/show", "", show_handler, NULL);	
  lo_server_add_method(osc_server, "/cutter/hide", "", hide_handler, NULL);	
  lo_server_add_method(osc_server, "/cutter/channel_led", "ii", 
			channel_led_handler, NULL);	
  //lo_server_add_method(osc_server, NULL, NULL, generic_handler, NULL);	

	//OSC addresses
	sc_send = lo_address_new(NULL, "57120" );	
	monome_send	= lo_address_new(NULL, "13090" );
	bridge_send	= lo_address_new(NULL, bridge_port );

	// State Initialization


	// Init leds ???

	// Main loop

	while(!quit)
		//lo_server_recv(osc_server);
		lo_server_recv_noblock(osc_server, 100);


	// Clean up.
	led_clear();
  //lo_server_thread_free(osc_server);
	return 0;
}




// F U N C T I O N   D E F I N I T I O N S

// Led Functions

void	led_clear()
{
	if(show == 1)
		lo_send(monome_send, "/cutter/grid/led/all", "i", 0);
}



// OSC Handlers

int 
generic_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{		
		int i;

		printf("path: <%s>\n", path);
		for (i = 0; i < argc; i++) {
			printf("arg %d '%c' ", i, types[i]);
			lo_arg_pp((lo_type)types[i], argv[i]);
			printf("\n");
		}

		printf("\n");
		fflush(stdout);
    return 0;		
}

int 
channel_led_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	char row = argv[0]->i;
	char led = argv[1]->i;

	if( led < 8 )
		lo_send(monome_send, "/cutter/grid/led/row", "iii", 
				0, row, 1 << led);

	else
		lo_send(monome_send, "/cutter/grid/led/row", "iiii", 
				0, 1, 0, 1 << (led-8));

}

int 
quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{		quit = 1;
    return 0;
}

int 
show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		hold_mask = 0;
		show = 1;
		//redraw leds
    return 0;
}

int 
hide_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		hold_mask = 0;
		show = 0;
    return 0;
}

void 
error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

//void control_row_press( char x ){
//	if( x < 8 ){	// Mute group press
//
//	}
//	//else if( x == 8 ) // Mute group edit
//	else if( x == 15 )	
//		quit = 1;
//
//}

void channel_row_press( char x, char y )
{
	float start = (float)x / 16;
	float end = 1;
	lo_send(sc_send, "/sampler/play", "iff", y, start, end);
}

int 
press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	char x, y, v;
	x = argv[0]->i;
	y = argv[1]->i;
	v = argv[2]->i;			
	printf("Press %u, %u\n", x, y);
	
	switch(mode)
	{
		case MODE_CUTTER:
			if( y == 0 )	// Control row
				puts("Control row press");
				//if( z == 1 )
				//	control_row_press(x);

			else					// One of the channel rows
				if(v == 1)
					channel_row_press(x, y);

			
		//case MODE_MUTE_GROUP_EDIT:
		//case MODE_SPEED_CHANGE:
		//case MODE_MIXER:
		//default:
	}
}

