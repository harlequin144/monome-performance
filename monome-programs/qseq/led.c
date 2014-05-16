// This file contains the contructs needed for maintaining the state of the leds
// on board this program. Maintaining things here makes updating leds
// monome-side easier and reduces osc traffic. 



static uint16_t led_row[8] 	= { 0 };
static uint16_t blinkers[8] = { 0 };




/* Led Functions */

void update_monome_leds(lo_address* monome){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		lo_send(monome, "/qseq/grid/led/row", "iiii", 0, i, led_row[i], led_row[i] >> 8);
}

void set_led_on(char x, char y){			led_row[y] 	|= 1 << x; 	}
void set_led_off(char x, char y){			led_row[y] 	&= ~(1 << x);	}
void set_blinker_on(char x, char y){	blinkers[y] |= (1 << x);	}
void set_blinker_off(char x, char y){	blinkers[y] &= ~(1 << x);	}

void clear_leds(lo_address* monome){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		led_row[i] = 0;

	lo_send(&monome, "/qseq/grid/led/all", "i", 0);
}

void clear_blinkers(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		blinkers[i] = 0;
}

void blink(lo_address* monome){
	//blinkey isnt working when updates are too frequent
	//I need a way to have blink's on off position be determined
	// regardless of the blink function.
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		led_row[i] ^= blinkers[i];

	update_monome_leds(monome);
}
