#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


enum trig_type
{
	trig,
	dub_tap,
	hold
};



struct Dtrig{

	// State
	char 	quit;	//0 = don't quit, all others = quit
	char 	show;	//0 = don't show, all others = show
	char 	trans; // 0 off, else on. when on, turn on the trans on next press
	int 	trans_press_cnt;
	int 	last_tick;

	int trig_press_cnt[16];
	int button_mask[8]; // for led lighting convienience
	char * drum_str[12]; 
		// for convieniently knowing which string to send and for assigning drums
		// to buttons

	// Osc Server - listening port
	lo_server osc_server;

	// Osc Addresses - talking ports
	lo_address sc_send;
	lo_address bridge_send;
	

	// Button info
	enum trig_type button_trig_type[12];

	// Holding info
	int hold_mask;
	int hold_point[12]; 
		// This is to keep track of when a holder was initialy pressed. Yes there
		// are probably more elements to the array than is necessary, since most
		// of the buttons will not be holders, but hey, I've got 8 jiggabights.

	//other/later
	//holder speed
	//quantization speed
	

};


void init_dtrig( struct Dtrig* dtrig){

	int i;

	dtrig->quit = 0;
	dtrig->show = 1;
	dtrig->trans = 0;
	dtrig->last_tick = 0;

	dtrig->hold_mask = 0;

	dtrig->trans_press_cnt = 0;
	for(i = 0; i < 16; i++)
		dtrig->trig_press_cnt[i] = 0;

	dtrig->button_mask[0] = 252;
	dtrig->button_mask[1] = 192;
	dtrig->button_mask[2] = 176;
	dtrig->button_mask[3] = 176;
	dtrig->button_mask[4] = 12;
	dtrig->button_mask[5] = 12;
	dtrig->button_mask[6] = 48;
	dtrig->button_mask[7] = 48;

	dtrig->drum_str[0] = "rid";		
	dtrig->drum_str[1] = "sn2";
	dtrig->drum_str[2] = "cow";
	dtrig->drum_str[3] = "cow";
	dtrig->drum_str[4] = "shk";
	dtrig->drum_str[5] = "sn1";
	dtrig->drum_str[6] = "sn2";	
	dtrig->drum_str[7] = "kk1";
	dtrig->drum_str[8] = "hhc";
	dtrig->drum_str[9] = "hhc";
	dtrig->drum_str[10] = "sn1";
	dtrig->drum_str[11] = "kk2";


	dtrig->button_trig_type[0] = trig;
	dtrig->button_trig_type[1] = dub_tap;
	dtrig->button_trig_type[2] = trig;
	dtrig->button_trig_type[3] = trig;
	dtrig->button_trig_type[4] = hold;
	dtrig->button_trig_type[5] = trig;
	dtrig->button_trig_type[6] = trig;
	dtrig->button_trig_type[7] = trig;
	dtrig->button_trig_type[8] = trig;
	dtrig->button_trig_type[9] = hold;
	dtrig->button_trig_type[10] = dub_tap;
	dtrig->button_trig_type[11] = trig;
}


