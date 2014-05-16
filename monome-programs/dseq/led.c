
//
// Design Note:
// These functions do nothing except make lighting the monome from other
// places in the program easier. NOTHING ELSE!
// The will not ever change the state of the program. They may however check
// their inputs for correctness of course.



//
// Convenience Functions
// 	

void 
light_col(struct Dseq* dseq, int x, char mask)
{
	lo_send(dseq->monome_send,"/dseq/grid/led/col", "iii", x,0,mask);
}

void 
light_row(struct Dseq* dseq, int x, char mask1, char mask2)
{
	lo_send(dseq->monome_send,
			"/dseq/grid/led/row", "iiii", 0, x, mask1, mask2);
		
	// /grid/led/row x_offset y s[..]
	// /grid/led/row 0 0 255 255
	// /grid/led/row 8 5 255
}

void 
light_button(struct Dseq* dseq, int x, int y, int onoff)
{
	lo_send(dseq->monome_send,
			"/dseq/grid/led/set", "iii", x, y, onoff);
}


void
light_clear(struct Dseq* dseq)
{
	lo_send(dseq->monome_send,"/dseq/grid/led/all", "i", 0);
}



//
// Main screen functions
//

void 
light_sequence_edit(struct Dseq* dseq)
	// Light up the sequence editing seciton.
	// Columns 0 thru 6
{
	if(dseq->show && dseq->screen == SCREEN_DSEQ)
	{
		int i, j;
		char mask[8];

		for(i = 0; i < 8; i++)
		{
			mask[i] = 0;
	
			// Sequence Edit
			if(i == 0)
				// Later on this part will be changed to show which pattern is playing
				mask[i] |= ~ONE_SHIFT(7);
			else
				for(j = 0; j < 7; j++)
					if( dseq->seq_showing != 0 )
					{
						if( dseq->seq[dseq->seq_showing-1][j] == i )
							mask[i] |= ONE_SHIFT(j);
					}
			
			// Sequence Select
			if(i != 0 && dseq->seq_showing != i)
				mask[i] |= ONE_SHIFT(7);

		}
		lo_send(dseq->monome_send, "/dseq/grid/led/map", "iiiiiiiiii", 0, 0, 
				mask[0], mask[1], mask[2], mask[3],
				mask[4], mask[5], mask[6], mask[7]);
	}
}

void
light_seqence_select(struct Dseq* dseq)
	// Light up the sequence selection column
	// Column 8
{
	if(dseq->show)
		light_col(dseq, 7, ~(ONE_SHIFT(dseq->seq_showing) | 1) );
}



void
light_dseq(struct Dseq* dseq)
{
	// We're gonna be a bad ass over here and minimize osc traffic by lighting
	// the entire monome with only 8 row messages.
	// All this logic is cheap compared to osc traffic.
	//
	// All this can again be done with just two messages using the map message!!
	if(dseq->show && dseq->screen == SCREEN_DSEQ)
	{
		
		int i, j;
		char mask1, mask2;
		for(i = 0; i < 8; i++)
		{
			mask1 = 0;
			mask2 = 0;
	
			// Sequence Edit
			if(i == 0)
				// Later on this part will be changed to show which pattern is playing
				mask1 |= ~ONE_SHIFT(7);
			else
				for(j = 0; j < 7; j++)
					if( dseq->seq[dseq->seq_showing-1][j] == i )
						mask1 |= ONE_SHIFT(j);
			
			// Sequence Select
			if(i != 0 && dseq->seq_showing != i)
				mask1 |= ONE_SHIFT(7);
	
	
			// Trigger section
			// three cols on the row of the seq playing are solid
			// next seq on col coresponding with loop mode blink
			if(i == dseq->seq_playing)
				mask2 |= ONE_SHIFT(3)-1;
			
			// Transport
			// Other stuff for start and stop bittons
			
			// Exit button
			if(i == 7)
				mask2 |= ONE_SHIFT(4);
			
			// Pattern edit row
			if(i != 0)
			if(dseq->pat[i-1].len != 0)
				mask2 |= ONE_SHIFT(7);
	
			light_row(dseq, i, mask1, mask2);
		}

	}
}

void
light_pattern_select_col(struct Dseq* dseq)
{
	char mask = 0;
	int i;
	for(i = 0; i < 7; i++)
		if(dseq->pat[i].len != 0)
			mask |= ONE_SHIFT(i+1);

	light_col(dseq, 15, mask);
}

void
light_transport(struct Dseq* dseq, char tick){
	// This function is meant to be able to draw over an already drawn main
	// screen in order to update the blinking transport section. So make sure
	// that it wont draw over anyting else


	if( dseq->show && dseq->screen == SCREEN_DSEQ)
	{
		if(tick >=0 && tick < TICKS_PER_BEAT/2 )	// down beat
		{
			light_col(dseq, 11, 1);
			light_col(dseq, 12, 1 | ONE_SHIFT(7));
			light_col(dseq, 13, 1);
		}
		else
		if( tick >= TICKS_PER_BEAT/2 && tick < TICKS_PER_BEAT + 1 )
		{
			light_col(dseq, 11, 2);
			light_col(dseq, 12, 2 | ONE_SHIFT(7));
			light_col(dseq, 13, 2);

		}
	}
}


//
// Pattern Edit Screen Functions
//

void
light_pattern_edit(struct Dseq* dseq)
{
	// This can make things even more efficient!!!
	// /grid/led/map 0 0 254 253 125 247 239 36 191 4
	if(dseq->show)
	if(dseq->screen > 0 && dseq->screen <= 7)
	{
		// Control Row
		light_row(dseq, 0, 
				ONE_SHIFT(dseq->pat[dseq->screen-1].len)-1,
				ONE_SHIFT(7) );
		
		// Pattern rows
		int i;
		for(i = 1; i < 8; i++)
		{
			light_row(dseq, i, 
					dseq->pat[dseq->screen-1].row[i-1], 
					dseq->pat[dseq->screen-1].row[i-1] >> 8);

		}
	}
}



