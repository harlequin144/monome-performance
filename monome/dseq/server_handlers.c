#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// FUNCTION PROTOTYPES

// Osc Server Prototypes
void error(int num, const char *m, const char *path);

int	 show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 hide_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 stop_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

int	 press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

	// Press handler helpers
		// main screen
		void main_screen_press		(struct Dseq* dseq, char x, char y);
		void pattern_edit_select_press(struct Dseq* dseq, char x, char y);

		//pattern edit screen
		void pattern_screen_press	(struct Dseq* dseq, char x, char y);

int	 tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

	// Tick handler helpers
	void beat_update_loop_beat(struct Dseq* dseq);
	void beat_update_loop_sequence(struct Dseq* dseq);
	void beat_update_loop_pattern(struct Dseq* dseq);
	void check_drum_trigger(struct Dseq* dseq, char tick);

int	 generic_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);



// F U N C T I O N   D E F I N I T I O N S

void 
error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

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
		printf("\nuserdata show:%c\n\n",
				((struct Dseq*) user_data)->show);

		printf("\n");
		fflush(stdout);
    return 0;		
}

int 
quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{		
		((struct Dseq*) user_data)->quit = 1;
    return 0;
}

int 
show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		struct Dseq* dseq = ((struct Dseq*) user_data);
		dseq->show = 1;
		dseq->transport_press_count = 0;
		//redraw leds
		if(dseq->screen == SCREEN_DSEQ)
			light_dseq(dseq);
		else
			light_pattern_edit(dseq);
    return 0;
}

int 
hide_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		((struct Dseq*) user_data)->show = 0;
		//Clear the monome?
    return 0;
}


int 
stop_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		struct Dseq* dseq = ((struct Dseq*) user_data);
		dseq->seq_playing_pos = 0;
		dseq->loop_mode = LOOP_SEQUENCE;
		dseq->beat = 0;
		//
		// Move relighting the transport section to here?
		//
    return 0;
}


int 
tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	// For convenience
	struct Dseq* dseq = ((struct Dseq*) user_data);
	char new_tick = argv[0]->i;

	// Error checking
	//if( (dseq->last_tick+1)%TICKS_PER_BEAT != new_tick )
		//printf("Report: last tick:%u, new tick%i\n", dseq->last_tick, new_tick);

	//if this is a down beat
	if( new_tick == 0)
	{
		dseq->beat++; 
		//printf("Down beat: %u\n", dseq->beat);
		
		if( dseq->seq_playing == 0) // Idle sequence
		{
			// Despite whatever the loop_mode is, whenever there is a different
			// sequence lined up to be played next, we will start it on the next
			// beat.
			dseq->seq_playing = dseq->next_seq;
			dseq->seq_playing_pos = 0;
			dseq->loop_mode = LOOP_SEQUENCE;
			dseq->beat = 0;
			check_drum_trigger(dseq, new_tick);
			if( dseq->show && dseq->screen == SCREEN_DSEQ)
			{
				light_col(dseq, 8, ONE_SHIFT(dseq->seq_playing));
				light_col(dseq, 9, ONE_SHIFT(dseq->seq_playing));
				light_col(dseq, 10, ONE_SHIFT(dseq->seq_playing));
			}
		}
		else
		{
			switch(dseq->loop_mode)
			{
				case LOOP_BEAT:
					beat_update_loop_beat(dseq);
					break;

				case LOOP_PATTERN:
					beat_update_loop_pattern(dseq);
					break;

				case LOOP_SEQUENCE:
					beat_update_loop_sequence(dseq);
					break;

				default:
					printf("Invalid loop_mode recieved in down_beat_update: %u\n", 
							dseq->loop_mode);
					break;
			}

			check_drum_trigger(dseq, new_tick);
		}
		light_transport(dseq, new_tick);

	}
	else // This is not a down_beat.
	{
		if( dseq->seq_playing != 0)
			check_drum_trigger(dseq, new_tick);

		if( new_tick == TICKS_PER_BEAT/2 )
			light_transport(dseq, new_tick);
		//If this is an up_beat, light the transport
	}

	//update the tick
	dseq->last_tick = argv[0]->i;

	return 0;
} // tick_handler


//
// trigger_handler helper functions
//

void 
check_drum_trigger(struct Dseq* dseq, char tick)
{
	if(tick%12 == 0 && dseq->seq_playing != 0)
	{
		int mask[7];
		int i;
		char found = 0;
		
		char play = dseq->seq_playing - 1;
		char pos = dseq->seq_playing_pos;

		if(dseq->pat[ dseq->seq[play][pos] - 1 ].len != 0)
		for(i = 0; i < 7; i++)
		{
			mask[i]= 0;
			if(
					(	dseq->pat[ dseq->seq[play][pos] - 1 ].row[i] & 
						ONE_SHIFT(tick/12 + (4 * dseq->beat) ) 					) != 0)
			{
				found = 1;
				mask[i] = i+1;
				//printf("Beat: %u\n", dseq->beat);
				//printf("%u ", mask[i]);
			}
		}
		if(found != 0)
		lo_send(dseq->sc_send, "/trigger", "iiiiiii",
				mask[0], mask[1], mask[2], mask[3], mask[4], mask[5], mask[6]);
	}
}


void 
beat_update_loop_beat(struct Dseq* dseq)
{
	// Convenience
	char sequ = dseq->seq_playing - 1;

	dseq->seq_playing = dseq->next_seq;
	dseq->seq_playing_pos = 0;
	if( dseq->show && dseq->screen == SCREEN_DSEQ)
	{
		light_col(dseq, 8, ONE_SHIFT(dseq->seq_playing));
		light_col(dseq, 9, ONE_SHIFT(dseq->seq_playing));
		light_col(dseq, 10, ONE_SHIFT(dseq->seq_playing));
	}
	//find the next nonempty pattern in the sequence
	dseq->seq_playing_pos++;
	while
	(
		dseq->seq_playing_pos < 7 && 
		(	dseq->seq[sequ][dseq->seq_playing_pos] == 0 ||
			dseq->pat[dseq->seq[sequ][dseq->seq_playing_pos] - 1].len == 0 )
	)
		dseq->seq_playing_pos++;

	dseq->loop_mode = LOOP_SEQUENCE;
	dseq->beat = 0;
} 

void 
beat_update_loop_pattern(struct Dseq* dseq)
{
	// Convenience
	char beat = dseq->beat;
	char sequ = dseq->seq_playing - 1;
	char posn = dseq->seq_playing_pos;

	if( beat >= dseq->pat[dseq->seq[sequ][posn] - 1].len )
	{
		dseq->seq_playing = dseq->next_seq;
		dseq->seq_playing_pos = 0;
		if( dseq->show && dseq->screen == SCREEN_DSEQ)
		{
			light_col(dseq, 8, ONE_SHIFT(dseq->seq_playing));
			light_col(dseq, 9, ONE_SHIFT(dseq->seq_playing));
			light_col(dseq, 10, ONE_SHIFT(dseq->seq_playing));
		}
		//find the first nonempty pattern in the sequence.
		dseq->seq_playing_pos++;
		while
		(
			dseq->seq_playing_pos < 7 && 
			(	dseq->seq[sequ][dseq->seq_playing_pos] == 0 ||
				dseq->pat[dseq->seq[sequ][dseq->seq_playing_pos] - 1].len == 0 )
		)
			dseq->seq_playing_pos++;

		dseq->loop_mode = LOOP_SEQUENCE;
		dseq->beat = 0;
	}
}


void 
beat_update_loop_sequence(struct Dseq* dseq)
{
	// Convenience
	char beat = dseq->beat;
	char sequ = dseq->seq_playing - 1;
	char posn = dseq->seq_playing_pos;

	if( beat >= dseq->pat[dseq->seq[sequ][posn] - 1].len )
	{
		// We have finished playing the pattern and it is time to find the next
		//printf("Current seq: %u, seq position: %u\n", 
		//		dseq->seq_playing, dseq->seq_playing_pos);
		//printf("Current pattern (should be 0-6): %u\n", 
		//		dseq->seq[dseq->seq_playing][dseq->seq_playing_pos] - 1);
		//printf("Current beat: %u, current pattern len: %u\n", beat, 
		//	dseq->pat[dseq->seq[sequ][posn] - 1].len );
		//puts("Time to switch to the next pattern!");

		//find the next pattern in the sequence or if this is the last
		//pattern in the current sequence
		dseq->seq_playing_pos++;
		while(	dseq->seq_playing_pos < 7 && 
						(	dseq->seq[sequ][dseq->seq_playing_pos] == 0 ||
							dseq->pat[dseq->seq[sequ][dseq->seq_playing_pos] - 1].len == 0 )
		)
			dseq->seq_playing_pos++;
		//printf("Next sequence position found: %u\n", dseq->seq_playing_pos);

		if( dseq->seq_playing_pos >= 7)	//We have reached the end of the seq
		{
			dseq->seq_playing = dseq->next_seq;
			
			// Need to search the seq to see where to start
			// and also deal with the case that the seq is empty
			
			dseq->seq_playing_pos = 0;
			while( 	dseq->seq_playing_pos < 7 && 
							(	dseq->seq[sequ][dseq->seq_playing_pos] == 0 ||
								dseq->pat[dseq->seq[sequ][dseq->seq_playing_pos] -1].len == 0 )
			)
				dseq->seq_playing_pos++;

			if( dseq->show && dseq->screen == SCREEN_DSEQ)
			{
				light_col(dseq, 8, ONE_SHIFT(dseq->seq_playing));
				light_col(dseq, 9, ONE_SHIFT(dseq->seq_playing));
				light_col(dseq, 10, ONE_SHIFT(dseq->seq_playing));
			}
			//printf("Hit end of seuqnce, Next sequence: %u\n",dseq->seq_playing);
		}
		//else 
		//	printf("Did not hit end of seuqnce.\n Next pattern: %u\n", 
		//			dseq->seq[dseq->seq_playing][dseq->seq_playing_pos]);

		dseq->beat = 0;
	}

}


int 
press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	char x, y, v;
	x = argv[0]->i;
	y = argv[1]->i;
	v = argv[2]->i;			
	struct Dseq* dseq = ((struct Dseq*) user_data);
	//printf("Press %u, %u\n", x, y);

	// Release
	if(v == 0)
	{
		if(dseq->screen == SCREEN_DSEQ)
			if(x == 14)	// shift row release
			{
				dseq->shift_mask = 0;
				light_col(dseq, 14, 0);
			}
			else if(x >= 11 && x <=13)
			{
				dseq->transport_press_count--;
				if(dseq->transport_press_count < 0 || dseq->transport_press_count > 10)
					dseq->transport_press_count = 0;
			}
	}

	// Press
	else
		if(dseq->screen == SCREEN_DSEQ)
			main_screen_press(dseq, x, y);

		else 
		if(dseq->screen < 8) // Neccesary to make this check?
			// Pattern edit only responds to presses
			pattern_screen_press(dseq, x, y);
		

	return 0;
}

//
// Press Handler Helper Functions
//

void 
pattern_screen_press(struct Dseq* dseq, char x, char y)
{
	if( y == 0 ) // Control row
	{
		if( x <= 3 )	// Beat length setting
		{
			dseq->pat[dseq->screen-1].len = x + 1;
			light_row(dseq, 0, ONE_SHIFT(x+1)-1, ONE_SHIFT(7));
		}
		else
		if( x == 15 )	// Exitback to main screen
		{
			dseq->screen = SCREEN_DSEQ;
			light_dseq(dseq);
		}

	}
	else
	{
		if(dseq->pat[dseq->screen-1].row[y-1] & ONE_SHIFT(x) )
				// button was in on position, turn it off
			dseq->pat[dseq->screen-1].row[y-1] &= ~ONE_SHIFT(x);
		else
			dseq->pat[dseq->screen-1].row[y-1] |= ONE_SHIFT(x);
			
		light_row(dseq, y, 
				dseq->pat[dseq->screen-1].row[y-1],
				dseq->pat[dseq->screen-1].row[y-1] >> 8);
	}
}

void
main_screen_press(struct Dseq* dseq, char x, char y)
{
	switch(x){
		// Sequence edit section
		case 0: case 1: case 2: case 3: case 4: case 5: case 6:
			
			if(y >= 1 && dseq->seq_showing != 0){
				//Later on this will be used to indicate which pattern is playing
				char mask = 1; 

				if( y == dseq->seq[dseq->seq_showing-1][x])
					dseq->seq[dseq->seq_showing-1][x] = 0;

				else
				{
					dseq->seq[dseq->seq_showing-1][x] = y;
					mask |= ONE_SHIFT(y);
				}

				light_col(dseq, x, mask);
			}
			break;
	

		// Sequence edit select
		case 7:
			dseq->seq_showing = y;
			light_sequence_edit(dseq);
			break;
	

		// Sequence looping
		case 8:
		case 9:
		case 10:
			dseq->loop_mode = x - 8;
			dseq->next_seq = y;
			if(dseq->seq_playing == y)
			{
				light_col(dseq, (8 + (dseq->loop_mode)), 0);
				light_col(dseq, (8 + (((dseq->loop_mode)+1)%3)), 
						ONE_SHIFT((dseq->seq_playing)));
				light_col(dseq, (8 + (((dseq->loop_mode)+2)%3)), 
						ONE_SHIFT(dseq->seq_playing));
			}
			else
			{
				light_col(dseq, (8 + (dseq->loop_mode)), ONE_SHIFT(y));
				light_col(dseq, (8 + (((dseq->loop_mode)+1)%3)), 
						ONE_SHIFT(dseq->seq_playing));
				light_col(dseq, (8 + (((dseq->loop_mode)+2)%3)), 
						ONE_SHIFT(dseq->seq_playing));
			}
			break;

			
		// Transport and Exit
		case 11:
		case 12:
		case 13:
			dseq->transport_press_count++;
			if(y >= 0 && y <= 3 && dseq->transport_press_count == 1)
			{
				switch(y)
				{
					case 0:
						lo_send(dseq->sc_send, "/sc/transport/start", NULL);
						break;
					case 1:
						lo_send(dseq->sc_send, "/sc/transport/stop", NULL);
						light_col(dseq, 11, 0);
						light_col(dseq, 12, ONE_SHIFT(7));
						light_col(dseq, 13, 0);
						break;
					case 2:
						lo_send(dseq->sc_send, "/sc/transport/tap", NULL);
						break;
					case 3:
						lo_send(dseq->sc_send, "/sc/transport/clear_tap", NULL);
						break;
				}
			}

			else
			if(y == 7 && x == 12){
				dseq->show = 0;
				lo_send(dseq->bridge_send, "/bridge/show", NULL);
			}
			
			break;

		// Shift row
		case 14:
			dseq->shift_mask = ONE_SHIFT(y);
			light_col(dseq, 14, dseq->shift_mask);
			break;

		// Pattern edit
		case 15:
			pattern_edit_select_press(dseq, x, y);
			break;
	}

} // main_screen_press

void
pattern_edit_select_press(struct Dseq* dseq, char x, char y)
{
			if(y != 0) // There is no 0th pattern
			if(dseq->shift_mask == 0)
			{
				// Just switch to edit mode for the selected pattern
				dseq->screen = y;
				if(dseq->pat[y-1].len == 0)
					dseq->pat[y-1].len = 4;

				light_pattern_edit(dseq);
			}
			else
			if(dseq->shift_mask == 1)
			{
				// User is holding the top shift button (on the top row, this button
				// does not correspond with a pattern). This will be used to quickly
				// clear out patterns without switching to them after wards. 
				
				clear_pattern(dseq, y-1);
				light_pattern_select_col(dseq);
			}
			else
			{
				if(dseq->shift_mask == ONE_SHIFT(y))
				{
					// switch to pattern edit on the deleted pattern
					clear_pattern(dseq, y-1);
					dseq->screen = y;
					dseq->pat[y-1].len = 4;
					dseq->shift_mask = 0;
					light_pattern_edit(dseq);
				}
				else{
					// Copy the pattern indicated by the shift mask to y and then switch
					// to that pattern for editing.
					
					int shift_index = 8;
					while(shift_index != 0 && ONE_SHIFT(shift_index) != dseq->shift_mask)
						shift_index--;

					if(shift_index != 0)
					{
						copy_pattern(dseq, shift_index-1, y-1);
						dseq->screen = y;
						dseq->shift_mask = 0;
						light_pattern_edit(dseq);
					}
					else
						puts("failed to find shift index, whiled to 0");
				}

				// what next?
			}
}
