/**
 *To Do:
 *-is it really necessary to have two listening sockets open?
 */

		//Also dont forget your idea on using adsr value to describe the
		//durations of the notes.

//#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "lo/lo.h"

#include "config.h"
#include "note_q.c"
#include "led.c"

//Constants
#define MODE_SEQ 1
#define MODE_EDIT 2
#define	TRIGGER_TICK	1	//Trigger points
#define	TRIGGER_BEAT	2
#define	TRIGGER_QUEUE	3
#define	TRIGGER_SEQ		4
#define	TRIGGER_IDLE	5
#define ON 1
#define OFF 0

float A[16]= 
{0, 73.42, 82.41, 92.5, 98.0, 110, 123.47, 138.59, 
146.83, 164.81,185.0, 196, 220, 246.94, 277.18, 293.66 };


/*
 * Global Variables
 */

// State Variables
char	quit 	= 0;
char 	tick 	= 0;
char 	mode 	= MODE_SEQ;		
char 	trigger_point = TRIGGER_IDLE;

// Q Variables
char 	show_Q = 7;
char 	show_seq = 7;
char 	play_seq = 7;
char 	next_play_seq = 7; 
char	play_seq_pos = 0;	//0-7
unsigned int play_Q_tick;

// THE Q SEQ..eq..eq..eq.......
unsigned short 	Q_seq [8][8];
unsigned int 		ticks_list[MONOME_Y_LEN] = { 4*TPB, 3*TPB, 2*TPB, 1*TPB, TPB/2, 
																						 TPB/4, TPB/8, TPB/16 }; //Top to bottom
Q_head 	 			Q[MONOME_Y_LEN];

lo_address 		bridge_send;
lo_address 		bass_send; 
lo_address 		monome_send;




//void update_leds(monome_t* m){
void update_leds(){
	clear_leds(monome_send);
	clear_blinkers(monome_send);
	int i;
	if(mode == MODE_SEQ){
		//Sequence side
		for(i = 0; i < 8; i++) //Light the sequence up
				set_led_on(i, Q_seq[show_seq][i]); 

		if(play_seq == show_seq)	//blink the current sequence.
			if(Q[Q_seq[show_seq][play_seq_pos]].len != 0)
				set_blinker_on(play_seq_pos, Q_seq[play_seq][play_seq_pos]);

		//Trigger/edit side
		for(i = 0; i < 8; i++){ 	//Light the 8,14,15 columns
			if(i != show_seq) 
				set_led_on(8, i);
			if(Q[i].len != 0)
				set_led_on(15, i);
		}
		set_blinker_on(14, Q_seq[play_seq][play_seq_pos]); //blink the playing Q

		if(trigger_point != TRIGGER_IDLE){
			for(i = 9; i < 13; i++)		//Blink the row of the playing seq
				set_blinker_on(i, play_seq); 
		}else
			set_led_on(13, 0);

	}

	if(mode == MODE_EDIT){
		set_blinker_on(0,0);
		for(i = 0;i<Q[show_Q].len/8; i++) //TPB/4
			set_led_on(i%MONOME_X_LEN, i/MONOME_X_LEN);
	}
		
	update_monome_leds(monome_send);
}

/*
 * Function Prototypes
 */

// Osc Server Prototypes
void error(int num, const char *m, const char *path);
int	 tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 note_change_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);




/*
 *Function Definitions
 */

void chill(int msec){
	struct timespec rem, req;

	req.tv_nsec  = msec * 1000000;
	req.tv_sec   = req.tv_nsec / 1000000000;
	req.tv_nsec -= req.tv_sec * 1000000000;

	nanosleep(&req, &rem);
}


int tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data){
	tick = argv[0]->i;
	//Might be beter to increment based on what the incoming tick number is
	//instead.
	//if(tick%32 == 0) //This was for if I am using a clock for the midi stuff
	//which will increment on the lcm of 32 and 24....
	play_Q_tick++;

	printf("tick :%u\n", tick);
	switch(trigger_point){
		case TRIGGER_BEAT:
			if(tick%32 != 0)	break; // when it is the beat, it will do the trigger_tick
		case TRIGGER_TICK:
			trigger_point = TRIGGER_SEQ;
			play_seq = next_play_seq;
			play_seq_pos = 0;
			play_Q_tick = 0;		
			char init_pos = play_seq_pos; 
			while(Q[Q_seq[play_seq][play_seq_pos]].len == 0){
				play_seq_pos = (play_seq_pos+1)%8;
				if(play_seq_pos == init_pos) break;
			}
			update_leds(monome_send);
			break;
		

		case TRIGGER_QUEUE:
			//puts("trig queue");
			if(Q[Q_seq[play_seq][play_seq_pos]].len != 0 &&
		     play_Q_tick >= Q[Q_seq[play_seq][play_seq_pos]].len )
			{		
				play_seq = next_play_seq;
				play_Q_tick = 0;
				play_seq_pos = 0;
				char init_pos = play_seq_pos;
				while(Q[Q_seq[play_seq][play_seq_pos]].len == 0){
					play_seq_pos = (play_seq_pos+1)%8;
					if(init_pos == play_seq_pos) break;
				}
				update_leds(monome_send);

				trigger_point = TRIGGER_SEQ;
			}
			break;

		case TRIGGER_SEQ:
			if(Q[Q_seq[play_seq][play_seq_pos]].len != 0 &&
		     play_Q_tick >= Q[Q_seq[play_seq][play_seq_pos]].len )
			{		
				play_Q_tick = 0;
				char init_pos = play_seq_pos;
				do{
					play_seq_pos = (play_seq_pos+1)%8;
					if(init_pos == play_seq_pos || 
						 play_seq_pos == 0) break;
				}while(Q[Q_seq[play_seq][play_seq_pos]].len == 0);

				if(play_seq_pos == 0){
					play_seq = next_play_seq;
				}

				update_leds(monome_send);
			}
			break;
	}	


	//now check to see if a note needs to be played.
	if(trigger_point != TRIGGER_IDLE){
		Q_node* tracer = Q[Q_seq[play_seq][play_seq_pos]].Q_ptr;	
		while(tracer != NULL){
			if(tracer->trig_tick == play_Q_tick){
				printf("note triggered: %u, %u\n", 
					tracer->note->pitch, tracer->note->tick_len);
				lo_send(bass_send, "/note", "f", A[tracer->note->pitch]);
				lo_send(bridge_send, "/qseq/note", "f", A[tracer->note->pitch]);
				break;
			}
			tracer = tracer->next;
		}
	}

	if(tick%16 == 0)
		blink(monome_send);

	return 0;
}


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
		update_leds();	
    return 0;
}

int	 
note_change_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	A[argv[0]->i] = argv[1]->f;
	return 0;
}

int press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data){

	unsigned int x, y, v;
	x = argv[0]->i;
	y = argv[1]->i;
	v = argv[2]->i;

	//printf("Press %u, %u\n", x, y);
	//lo_send(bridge_send, "/qseq/press", "ii", x,y);
	
	if(v == 1) //Down presses only
	switch(mode){
		case MODE_SEQ:
			switch(x){
				case 0:case 1:case 2:case 3:
				case 4:case 5:case 6:case 7:
					Q_seq[show_seq][x] = y;
					update_leds();	
					break;
				case 8:		//Show the sequence
					//set_blinker_off(8,show_seq);
					clear_blinkers();
					show_seq = y;
					set_blinker_on(8,y);
					update_leds();
					break;
				case 9:		//Play the sequence when the current seq is complete
					trigger_point = TRIGGER_SEQ;
					next_play_seq = y;
					break;
				case 10:	//Play this sequence when the current Q is complete
					trigger_point = TRIGGER_QUEUE;
					next_play_seq = y;
					break;
				case 11:	//Play this sequence on the next beat 
					trigger_point = TRIGGER_BEAT;
					next_play_seq = y;
					break;
				case 12: 	//Play this sequence on next tick
					trigger_point = TRIGGER_TICK;
					next_play_seq = y;
					break;
				case 13:
					if(y == 7){
						clear_leds(monome_send);
						lo_send(monome_send, "/sys/prefix", "s", "bridge");
						lo_send(bridge_send, "/bridge/show", NULL);
					}
					if(y == 0){
						trigger_point = TRIGGER_IDLE;
						update_leds(monome_send);
					}
					break;
				case 14: //Delete the contents of this queue
					clearqueue((Q + y));
				case 15: //Open this queue for editing
					mode = MODE_EDIT;
					show_Q = y;
					clear_leds(monome_send);
					update_leds(monome_send);
					break;
			}
			break;

		case MODE_EDIT:
			if(x == 0 && y == 0){	// The exit button
				mode = MODE_SEQ;	
				update_leds(monome_send);
			}	else if(x == 0 && y == 7){
				dequeue((Q+show_Q));
				update_leds(monome_send);
			}else{
				enqueue((Q+show_Q), x, ticks_list[y]); 
				update_leds(monome_send);
			}
			break;
			
		default:
			puts("invalid mode called in switch");
	}	
}

void error(int num, const char *msg, const char *path){
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}



int main() {
	//Binding to the osc port shall serve as the single instance insurance
	//mechanism. 

	/*
	 * Initialization
	 */

	//Initialize OSC Server
  lo_server osc_server = lo_server_new("7720", error);	
	if(!osc_server){
		puts("Exiting. Failed to create server.");
		lo_send(bridge_send, "/qseq/quiting", "s", 
				"Exiting. Failed to create server thread.");
		exit(1);
	}

  lo_server_add_method(osc_server, "/tick", "i", tick_handler, NULL);	
  lo_server_add_method(osc_server, "/qseq/grid/key","iii", press_handler, NULL);	
  lo_server_add_method(osc_server, "/qseq/change_note", "if", note_change_handler, NULL);
  lo_server_add_method(osc_server, "/qseq/quit", "", quit_handler, NULL);	
  lo_server_add_method(osc_server, "/qseq/show", "", show_handler, NULL);	
  //lo_server_add_method(osc_server, "/qseq/hide", "", show_handler, NULL);	
  //lo_server_add_method(osc_server, "/qseq/enque", "", show_handler, NULL);	
  //lo_server_add_method(osc_server, "/qseq/init", "", show_handler, NULL);	
	

	//Init OSC addresses
	bridge_send = lo_address_new(NULL, "8000" );	
	bass_send 	= lo_address_new(NULL, "57120" );
	monome_send	= lo_address_new(NULL, "13090" );

	clear_leds(monome_send);
		

	//Q Initialization
	int i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)	Q_seq[i][j] = i;

	for(i = 0; i < MONOME_Y_LEN; i++){
		Q[i].Q_ptr = NULL;
		Q[i].len = 0;
	}

	clear_leds(monome_send);
	update_leds(monome_send);


	/*
	 * Main loop
	 */

	while(!quit) {
		//lo_server_recv(osc_server);
		lo_server_recv_noblock(osc_server, 10);

		// Make sure that the current q is not empty
		// search for the next open queue and set the queue counter to 0.
		//This must be handled here because this case must be handled quickly
		char initial_position = play_seq_pos;
		while(Q[Q_seq[play_seq][play_seq_pos]].len == 0){
			play_seq_pos = (play_seq_pos+1)%8;
			if(play_seq_pos == initial_position) break;
		}

		//chill(10);
	}

	// Clean up.
	//Clean the queues
	clear_leds(monome_send);
	//monome_close(monome_send);
  //lo_server_thread_free(osc_server);
	return 0;
}
