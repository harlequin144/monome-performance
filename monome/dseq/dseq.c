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

void parse_options(int argc, char **argv, struct Dseq *dseq);

// MAIN FUNCTION

int 
main(int argc, char **argv)
{
	// State Initialization
	struct Dseq dseq;
	init_dseq(&dseq);

	parse_options(argc, argv, &dseq);


	// Register osc handler methods
  lo_server_add_method(dseq.osc_server,"/dseq/grid/key","iii", 
																												press_handler, &dseq);
 	lo_server_add_method(dseq.osc_server,"/dseq/quit", "",quit_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/dseq/show", "",show_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/dseq/hide", "",hide_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/dseq/stop", "",stop_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/tick", "i", tick_handler, &dseq);	
  //lo_server_add_method(osc_server, NULL, NULL, generic_handler, &dseq);	

	
	light_dseq(&dseq);

	// Main loop
	

	while(!dseq.quit)
		lo_server_recv(dseq.osc_server);
		//lo_server_recv_noblock(dseq.osc_server, 100);


	// Clean up.
	light_clear(&dseq);
  //lo_server_thread_free(osc_server);
	return 0;
}




// F U N C T I O N   D E F I N I T I O N S

void parse_options(int argc, char **argv, struct Dseq *dseq){
	int i=0;
	
	char* listen_port = "9999";
	char* monome_port = "13090";
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
	dseq->sc_send 		= lo_address_new(NULL, "57120" );	
	dseq->monome_send = lo_address_new(NULL, "13090" );	
	dseq->bridge_send	= lo_address_new(NULL, bridge_port );
	
	// Osc Server
	dseq->osc_server = lo_server_new(listen_port, error);	
	if(!dseq->osc_server){
		puts("Exiting. Failed to create server.");
		exit(1);
	}
}
