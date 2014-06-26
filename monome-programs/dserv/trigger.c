#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "lo/lo.h"

#define ONE_SHIFT(a) (1 << a)
#define TPB 48
	//TPB == ticks per beat

//#include "data_structs.c"
//#include "led.c"
//#include "server_handlers.c"



// FUNCTION PROTOTYPES

void parse_options(int argc, char **argv, struct Dseq *dseq);

// MAIN FUNCTION

int 
main(int argc, char **argv)
{
	// State Initialization

	parse_options(argc, argv, &dseq);


	// Register osc handler methods
  lo_server_add_method(dseq.osc_server,"/trigger/grid/key","iii", 
																												press_handler, &dseq);
 	//lo_server_add_method(dseq.osc_server,"/trigger/quit", "",quit_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/trigger/show", "",show_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/trigger/hide", "",hide_handler, &dseq);	
	lo_server_add_method(dseq.osc_server,"/trigger/stop", "",stop_handler, &dseq);	
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
Trigger {

	//const path = "/sc/trigger";
	//const lPath = "/sc/trigger/grid/led";
	//const up_map = Array[
	//const down_map = Array[

	// State
	var show = false;
	var trans_press_count = 0;
	var show_map; // = Array[0,0,0,0,0,0,0,0]
		// This structure is meant to make lighting up the monome easy.
	var speed_selector // Array[
		// This structure keeps track of the edge button speeds.

	// Osc/Midi
	var bridge;


	*new {|bridgePortNum = 8000|
		^super.new.init(midiUid, bridgePortNum)
	}

	init {|bridgePortNum = 8000|
		bridge = NetAddr.new("localhost", bridgePortNum);

		// Data Structure Initialization

		// Osc Responder Registration
		OSCdef(\micronome_press,
			{|msg, time| this.pressResponder(msg[1], msg[2], msg[3]) },
			path +/+ 'grid/key');

		OSCdef(\micronome_hide, { show = false; }, path+/+'hide');
		OSCdef(\micronome_tick, {|msg| this.tickResponder(msg[1]) }, '/tick');
		OSCdef(\micronome_show, { show = true; this.show;}, path+/+'show');
	}


	/*
	 * Press Functions
	 */
	pressResponder {|xPos, yPos, zPos|
		show = true;

		if( xPos >= 8 ){
			this.trans_press(xPos, yPos)
		}{
			if( zPos == 1 ){
				this.trigger_press(xPos, yPos)
			}
		}
	}

	trigger_press {|xPos, yPos
		case

		{ (yPos > 4) && (xPos < 11) }
		{
			// 3 X 3 of non-repeating triggers
		}

		{ (yPos == 4) && (xPos < 12)}
		{
			// horizontal repeaters and the corner one
		}

		{ (yPos > 4) && (xPos == 11)}
		{
			// vertical repeaters
		}


		{ (yPos <= 2) && (xPos < 11) }
			// The top three speed setters
		{
		}

		{ (yPos > 4) && (xPos > 12) }
		{
			// The side speed setters
		}

		{ (yPos <= 2) && (xPos > 12) }
		{
			// The diagonal speed setter
			if( xPos == yPos){
			}
		}

	}

	trans_press {|xPos, yPos|
		case
		// Toggle
		{ yPos == 0 }
		{
		}

		// Tap and Clear
		{ (yPos >= 1) && (yPos <= 4) }
		{
			if( xPos <= 3 )
			{ ~trans.clear_tap }
			{ ~trans.tap(time) }
		}

		// Exit
		{ (yPos == 7) && ( xPos == 0)}
		{
		}

	}

	/*
	 * Press helpers
	 */


	/*
	 * Other Functions
	 */
	transToggle {
		if( ~trans.on ){
			~trans.stop;
			bridge.sendMsg(lPath +/+ "set", 2,0,0);
			bridge.sendMsg(lPath +/+ "set", 3,0,0);
			bridge.sendMsg(lPath +/+ "row",0,1, 80,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 0);
			if(seqState == 2){ this.killNotesOn() };
		}{
			~trans.start;
			bridge.sendMsg(lPath +/+ "set", 2,0,1);
			bridge.sendMsg(lPath +/+ "set", 3,0,1);
		}
	}


	/*
	 * Hide n show
	 */
	show { // Light up the monome
		var seqFact = if(seqState == 2){3}{seqState};
		var pitchG4 = (pitchRange >=5 );
		var selState = (selectedSeq <2);
		// Left side
		bridge.sendMsg(lPath +/+ "map", 0,0,
			( pitchG4.if{241}{ 1+(16*((2**pitchRange)-1)) } ) +
			( if(~trans.on){12}{0} ),
			80, 0, // Tap and clear - let tick handle update this
			80 + (4*seqFact), // Play and Record
			80 + (4*seqFact),
			80 + (2** selectedSeq), // Select Seq
			2**seqPlaySpeed,//Playback speed
			83 + (9*hold.if{1}{0}) // Hold
		);
		// Right Side
		bridge.sendMsg(lPath +/+ "map",8,0,
			(pitchG4).if{ 2**(pitchRange-4)-1 }{0},
			171,0,171,171,171,0,171);
	}

	hide{
		show = false;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		controlPressCount = 0;
	}

}
