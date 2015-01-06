#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// Modes
#define SCREEN_DSEQ						0	// Main mode

// Looping Modes
#define LOOP_BEAT			0
#define LOOP_PATTERN	1
#define LOOP_SEQUENCE	2


struct Pattern{
	unsigned int row[7];
	char len;
};


struct Dseq{

	// State
	char quit;	//0 = don't quit, all others = quit
	char show;	//0 = don't show, all others = show
	char screen; 
	// screen is used to keep track of what mode the program is in (main screen or
	// pattern edit mode) as well as which pattern is being edited when we go
	// into pattern edit mode. 0 means nothing is being edited and we are in the
	// normal main mode. 1-7 correspond with each of the 7 patterns (so when
	// screen is 3 for example we are editing pattern 2).


	// Patterns/sequences
	struct Pattern pat[7];
		// Although on the monome, the 7 patterns will corespond with rows 1 - 7,
		// the patterns will be refered to by the numbers 0 - 6. 
	char seq[7][7];					
		// There are 7 sequences, each sequencing up to 7 patterns. Each element
		// in this 2d array stores each selected pattern as such:
		//
		// 	seq[ which seq ][ position in seq ] = pattern number + 1
		//
		// Pattern number is stored plus one because 0 is reserved to mean no
		// pattern is selected.

	char seq_showing;				// Track which seq is shown in the editor section
	char seq_playing;				// Track which seq is currently being played
	char seq_playing_pos;		// Track the position in playing seq
		//Sequences are named using numbers 0-7. 0 is the idle sequence.
		//Activating it will be telling dseq to do nothing. The other 1-7
		//sequences correspong with the numbering of the rows of the monome, but,
		//since there are actually only 7, they are accessed using the naming
		//number minus 1.

	// Time and Scheduling
	char next_seq;					// Which sequence is coming next
	char loop_mode;					// When will the next sequence be played?
	char last_tick;
	char beat;							
		// Track the beat within that pattern. Beats are tracked starting from 0.
		// So the 2nd beat for example is stored here as 1. Thus when a pattern is
		// x beats long and we are on beat x, the pattern has been completely
		// played and we should now be at the very beginnning of the next pattern.

	// Button book keeping
	unsigned int shift_mask;
	char transport_press_count; 
	// transport_press_count is used to keep track of the number of transport
	// buttons that are being pressed in order prevent accidental double presses.
	// This is to make it so that I can mash the transport buttons and not worry
	// too much about accidental presses.


	// Osc Server - listening port
	lo_server osc_server;

	// Osc Addresses - talking ports
	lo_address sc_send;
	lo_address bridge_send;
	lo_address monome_send;
};


void init_dseq( struct Dseq* dseq){

	dseq->quit = 0;
	dseq->show = 1;
	dseq->screen = 0;

	dseq->seq_showing = 0;
	dseq->seq_playing = 0;
	dseq->seq_playing_pos = 0;
	dseq->beat = 0;
	dseq->next_seq = 0;
	
	dseq->loop_mode = 2;
	dseq->last_tick = 0;
	dseq->shift_mask = 0;
	dseq->transport_press_count = 0;

	int i, j;
	for(i = 0; i < 7; i++){
		dseq->pat[i].len = 0;

		for(j = 0; j < 7; j++){
			dseq->pat[i].row[j]	= 0;
			dseq->seq[i][j] = 0;
		}
		dseq->seq[i][0] = i+1;
	}

}


void 
clear_pattern(struct Dseq* dseq, char pattern_num)
{
	if( pattern_num >= 0 && pattern_num < 7 )
	{
		int i;
		for(i = 0; i < 8; i++)
			dseq->pat[pattern_num].row[i] = 0;

		dseq->pat[pattern_num].len = 0;
	}
	else
		puts("atempted to clear patern at index out of range");
}


void 
copy_pattern(struct Dseq* dseq, char copy_from_num, char copy_to_num)
{
	if( copy_from_num >= 0 && copy_from_num < 7 &&
		 	copy_to_num >= 0 	&& copy_to_num < 7)
	{
		int i;
		for(i = 0; i < 7; i++)
			dseq->pat[copy_to_num].row[i] = dseq->pat[copy_from_num].row[i];
	
		dseq->pat[copy_to_num].len = dseq->pat[copy_from_num].len;

	}
	else
		puts("atempted to copy from patern at index out of range");

}



