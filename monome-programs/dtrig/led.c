
//
// Design Note:
// These functions do nothing except make lighting the monome from other
// places in the program easier. NOTHING ELSE!
// The will not ever change the state of the program. They may however check
// their inputs for correctness of course.



//
// Convenience Functions
// 	

//void 
//light_col(struct Dtrig* dtrig, int x, char mask)
//{
//	lo_send(dtrig->monome_send,"/dtrig/grid/led/col", "iii", x,0,mask);
//}
//
//void 
//light_row(struct Dtrig* dtrig, int x, char mask1, char mask2)
//{
//	lo_send(dtrig->monome_send,
//			"/dtrig/grid/led/row", "iiii", 0, x, mask1, mask2);
//		
//	// /grid/led/row x_offset y s[..]
//	// /grid/led/row 0 0 255 255
//	// /grid/led/row 8 5 255
//}
//
//void 
//light_button(struct Dtrig* dtrig, int x, int y, int onoff)
//{
//	lo_send(dtrig->monome_send,
//			"/dtrig/grid/led/set", "iii", x, y, onoff);
//}
//
//
//void
//light_clear(struct Dtrig* dtrig)
//{
//	lo_send(dtrig->monome_send,"/dtrig/grid/led/all", "i", 0);
//}
//


void
light_dtrig(struct Dtrig* dtrig)
{
	int control_mask[8] = {0,0,0,0,0,0,0,1};

	lo_send(dtrig->bridge_send,"/dtrig/grid/led/map", "iiiiiiiiii", 0, 0,
			control_mask[0], control_mask[1],
			control_mask[2], control_mask[3],
			control_mask[4], control_mask[5],
			control_mask[6], control_mask[7]);

	lo_send(dtrig->bridge_send,"/dtrig/grid/led/map", "iiiiiiiiii", 8,0,
		dtrig->button_mask[0], dtrig->button_mask[1], 
		dtrig->button_mask[2], dtrig->button_mask[3], 
		dtrig->button_mask[4], dtrig->button_mask[5], 
		dtrig->button_mask[6], dtrig->button_mask[7]	);
}
