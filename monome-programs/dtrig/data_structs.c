#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


enum trig_type
{
	press_shot,
	hold
};



struct Dtrig{

	// State
	char quit;	//0 = don't quit, all others = quit
	char show;	//0 = don't show, all others = show
	char trans; //0 means off, else is on
	int trans_press_cnt;
	int trig_press_cnt[16];
	int button_mask[8];

	// Osc Server - listening port
	lo_server osc_server;

	// Osc Addresses - talking ports
	lo_address sc_send;
	lo_address bridge_send;
	

	// Button info
	enum trig_type button_trig_type[16];
	int trig_up_drum_num[16];
	int trig_dn_drum_num[16];

	// Quantization and holding info
	int trig_mask;
	int hold_mask;

	//button mask

	//other/later
	//holder speed
	//quantization speed
	

};


void init_dtrig( struct Dtrig* dtrig){

	int i;

	dtrig->quit = 0;
	dtrig->show = 1;
	dtrig->trans = 0;

	dtrig->trig_mask = 0;
	dtrig->hold_mask = 0;

	dtrig->trans_press_cnt = 0;
	for(i = 0; i < 16; i++)
		dtrig->trig_press_cnt[i] = 0;

	dtrig->button_mask[0] = 195;
	dtrig->button_mask[1] = 195;
	dtrig->button_mask[2] = 48;
	dtrig->button_mask[3] = 48;
	dtrig->button_mask[4] = 12;
	dtrig->button_mask[5] = 12;
	dtrig->button_mask[6] = 195;
	dtrig->button_mask[7] = 195;

	//Press
	dtrig->trig_dn_drum_num[0] = 7;		//ride
	dtrig->trig_dn_drum_num[1] = 13;	//cow
	dtrig->trig_dn_drum_num[2] = 1;		//snare 2 double
	dtrig->trig_dn_drum_num[3] = 0;		//snare 1
	dtrig->trig_dn_drum_num[4] = 6;		//shaker hold
	dtrig->trig_dn_drum_num[5] = 6;		//shaker
	dtrig->trig_dn_drum_num[6] = 1;		//snare 2
	dtrig->trig_dn_drum_num[7] = 0;		//snare 1
	dtrig->trig_dn_drum_num[8] = 2;		//kick 1
	dtrig->trig_dn_drum_num[9] = 3;		//kick 2
	dtrig->trig_dn_drum_num[10] = 4;	//hhc
	dtrig->trig_dn_drum_num[11] = 5;	//hhc holder
	dtrig->trig_dn_drum_num[12] = 2;	//kick 1
	dtrig->trig_dn_drum_num[13] = 4; 	//hhc
	dtrig->trig_dn_drum_num[14] = 5;	//hho
	dtrig->trig_dn_drum_num[15] = 5;	//hho holder

	//Release
	dtrig->trig_up_drum_num[0] = -1;
	dtrig->trig_up_drum_num[1] = -1;
	dtrig->trig_up_drum_num[2] = 1;		//snare 2
	dtrig->trig_up_drum_num[3] = -1;	
	dtrig->trig_up_drum_num[4] = -1;
	dtrig->trig_up_drum_num[5] = -1;
	dtrig->trig_up_drum_num[6] = -1;
	dtrig->trig_up_drum_num[7] = 0;		//snare 1
	dtrig->trig_up_drum_num[8] = 2;		//kick 1
	dtrig->trig_up_drum_num[9] = -1;
	dtrig->trig_up_drum_num[10] = -1;
	dtrig->trig_up_drum_num[11] = -1;
	dtrig->trig_up_drum_num[12] = -1;
	dtrig->trig_up_drum_num[13] = 5; 	//hho
	dtrig->trig_up_drum_num[14] = -1;
	dtrig->trig_up_drum_num[15] = -1;
}


