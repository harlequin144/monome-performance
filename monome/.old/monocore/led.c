


uint16_t led_row[8] = { 0 };
uint16_t blinkers[8] = { 0 };


void update_row( unsigned char i){
	monome_led_row(monome, 0, i, 2, (uint8_t *) &(led_row[i]));
}

void update_leds(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		update_row(i);
}

void set_led_on(char x, char y){
		led_row[y] |= 1 << x;
}

void set_led_off(char x, char y){
		led_row[y] &= 1 << x;
}

void set_blinker_on(char x, char y){
		blinkers[y] |= 1 << x;
}

void set_blinker_off(char x, char y){
		blinkers[y] &= 1 << x;
}
