#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "lo/lo.h"
//#include "config.h"

#define YMASK 255


// GLOBAL VARIABLES

// State
char quit = 0;
char mode = 0; //0 means course mode, else is fine mode
char show = 0; //0 don't show, 1 show
char course[16]; 
char fine[16][7];
int  hold_mask = 0;
char last_press[16];

// Ports
lo_address monome_send;
lo_address sc_send;
lo_address bridge_send;


// FUNCTION PROTOTYPES

//General Prototypes
void led_clear();
void led_light_course();
void send_new_slider_position(int x, int y);

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

  lo_server_add_method(osc_server, "/slider/grid/key","iii", press_handler, NULL);
  lo_server_add_method(osc_server, "/slider/quit", "", quit_handler, NULL);	
  lo_server_add_method(osc_server, "/slider/show", "", show_handler, NULL);	
  lo_server_add_method(osc_server, "/slider/hide", "", hide_handler, NULL);	

	//OSC addresses
	sc_send = lo_address_new(NULL, "8888" );	
	monome_send	= lo_address_new(NULL, "13090" );
	bridge_send	= lo_address_new(NULL, bridge_port );

	// State Initialization
	for(i = 0; i < 16; i++){
		course[i] = 7;
		last_press[i] = 0;
		for(j = 0; j < 7; j++)
			fine[i][j] = 7;
	}

	led_light_course();


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
// 	Led functions are not coupled to the state of the actual slider. These are
// 	convienience functions that are used to do that coupling.

void	led_clear()
{
	if(show == 1)
		lo_send(monome_send, "/slider/grid/led/all", "i", 0);
}

void	led_light_column(int col, int pos)
{
	if(show == 1){
		int i=0, mask = 0;
		for(i; i < pos; i++)
			mask |= (1 << i);
		lo_send(monome_send, "/slider/grid/led/col", "iii", col, 0, ~mask & YMASK);
	}
}

void 	led_light_course()
{
	if(show == 1){
		led_clear();
		int j;
		for(j = 0; j < 16; j++)
			led_light_column(j, course[j]);
	}
}


void 
send_new_slider_position(int x, int y)
{
	course[x] = y;
	//send message
	
	//lo_send(monome_send, "/slider/grid/led/col", "iii", x, 0, 255-mask);
	led_light_column(x, y);
}


// OSC Handlers

int 
quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
    quit = 1;
    return 0;
}

int 
show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		hold_mask = 0;
		show = 1;
		led_light_course();	
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
handle_control(int y, int v){
	if(v == 1) //Press
		if(hold_mask & (1 << 15) ){ //is already being held down
			puts("hide slider");
			//send show to bridge
			//lo_send(bridge_send, "/bridge/show", NULL);
			//show = 0;
			//This is not a good idea because we can only exit by changing all of the
			//channels... I'll rely on the bridge's backspace key for now. 
			

		}else{
			hold_mask |= (1 << 15);
			int i;
			for(i = 0; i <= 15; i++)
				send_new_slider_position(i, y);
		}
	else
		hold_mask &= ~(1 << 15);
}


int 
press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	unsigned int x, y, v;
	x = argv[0]->i;
	y = argv[1]->i;
	v = argv[2]->i;			
	//printf("Press %u, %u\n", x, y);
	
	switch(mode){
		case 0: //course mode
			if(x == 15)
				handle_control(y,v);

			//No presses before and now pressing
			else if( (~hold_mask & (1 << x)) && v == 1){ 
				hold_mask |= (1 << x);
				last_press[x] = y;
				send_new_slider_position(x, y);
			}
			//else if( (~hold_mask & (1 << x)) && v == 0)	//no presses and releasing
				//hold_mask &= ~(1 << x);
		
			//else if( (hold_mask & (1 << x)) && v == 0) //one press and releasing
			else if( v == 0 ) //one press and releasing
				hold_mask &= ~(1 << x);
			
			//One press before and now pressing again
			else if( (hold_mask & (1 << x)) && v == 1 && last_press[x] != 0){	
				mode = 1;
				hold_mask = (1 << x);
				led_clear();
				led_light_column(x, fine[x][last_press[x]]);
			}

			break;

		default: //fine mode
			if( v == 1 ){
				//find channel being changed
				int channel = 0;
				while(hold_mask != (1<<channel) && channel < 17)
						channel++;
				fine[channel][last_press[channel]] = y;

				//update leds
				led_clear();
				led_light_column(channel, fine[channel][last_press[channel]]);
 
				//Here hold mask has a 1 while releases before the press are made.
				hold_mask = 0; 

				//send message


			}else if( (~hold_mask & (1 << x)) && v == 0 ){
				mode = 0;
				//redraw nomal mode
				led_light_course();
				hold_mask = 0;
			}
	}
}

void 
error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}
