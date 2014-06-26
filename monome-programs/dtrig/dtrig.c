#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "lo/lo.h"

#define ONE_SHIFT(a) (1 << a)
#define TICKS_PER_BEAT 48

#include "data_structs.c"
#include "led.c"
#include "server_handlers.c"



// FUNCTION PROTOTYPES

void parse_options(int argc, char **argv, struct Dtrig *dtrig);

// MAIN FUNCTION

int 
main(int argc, char **argv)
{
	// State Initialization
	struct Dtrig dtrig;
	init_dtrig(&dtrig);

	parse_options(argc, argv, &dtrig);


	// Register osc handler methods
  lo_server_add_method(
			dtrig.osc_server,"/dtrig/grid/key","iii", press_handler, &dtrig);
 	lo_server_add_method(dtrig.osc_server,"/dtrig/quit","",quit_handler,&dtrig);	
	lo_server_add_method(dtrig.osc_server,"/dtrig/show","",show_handler,&dtrig);	
	lo_server_add_method(dtrig.osc_server,"/dtrig/stop","",stop_handler,&dtrig);	
	lo_server_add_method(dtrig.osc_server,"/dtrig/hide","",hide_handler,&dtrig);	
	lo_server_add_method(dtrig.osc_server,"/tick", "i", tick_handler, &dtrig);	
  lo_server_add_method(dtrig.osc_server, NULL, NULL, generic_handler, &dtrig);	

	
	//light_dtrig(&dtrig);

	// Main loop
	while(!dtrig.quit)
		lo_server_recv(dtrig.osc_server);
		//lo_server_recv_noblock(dtrig.osc_server, 100);


	// Clean up.
  //lo_server_thread_free(osc_server);
	return 0;
}




// F U N C T I O N   D E F I N I T I O N S

void parse_options(int argc, char **argv, struct Dtrig *dtrig){
	int i=0;
	
	char* listen_port = "9999";
	char* bridge_port = "8000";

	char option;
	opterr = 0;

	while((option = getopt(argc, argv, "l:")) != -1)
		switch(option){
			case 'l': //Listen
				//Check to make sure that it is a valid port here.
				listen_port = optarg;
				break;

			case 'b': //Bridge
				bridge_port = optarg;
				break;

			case '?':
				if(optopt == 'p')
	 				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      	else if (isprint (optopt))
	 		  	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      	else
	 		  	fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
      	//return 1;
	 	  default:
				abort();
		}
	
  printf("listen port = %s, \n",listen_port);
	       
  if(i < argc)
 	   printf ("Non-option argument %s\n", argv[i]);
	

	// Osc addresses
	dtrig->sc_send 		= lo_address_new(NULL, "57120" );	
	dtrig->bridge_send	= lo_address_new(NULL, bridge_port );
	
	// Osc Server
	dtrig->osc_server = lo_server_new(listen_port, error);	
	if(!dtrig->osc_server){
		puts("Exiting. Failed to create server.");
		exit(1);
	}
}
