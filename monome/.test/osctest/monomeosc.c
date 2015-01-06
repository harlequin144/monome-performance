#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <monome.h>
#include "lo/lo.h"


unsigned int grid[16][16] = { [0 ... 15][0 ... 15] = 0 };

#define MONOME_DEVICE "osc.udp://127.0.0.1:13090/monome"
#define PD_SEND "osc.udp://127.0.0.1:7771/note"


void send_note(unsigned int a, unsigned int b){
	lo_address send_address = lo_address_new_from_url(PD_SEND);

	if( lo_send(send_address, "/play", "ii", a, b) == -1)
		puts("failed to send\n");
}	

/**
 * this function gets registered with monome_register_handler
 * it gets called whenever a button is pressed
 */
void handle_press(const monome_event_t *e, void *data) {
	unsigned int x, y;

	puts("presshandle");

	x = e->grid.x;
	y = e->grid.y;

	send_note(x,y);
	/* toggle the button */
	grid[x][y] = !grid[x][y];
	monome_led_set(e->monome, x, y, grid[x][y]);
}

int main(int argc, char *argv[]) {
	puts("program running");

	monome_t *monome;


	/* open the monome device */
	if( !(monome = monome_open(MONOME_DEVICE, "8000")) )
		return -1;

	monome_led_all(monome, 0);

	/* register our button press callback */
	monome_register_handler(monome, MONOME_BUTTON_DOWN, handle_press, NULL);

	/* wait for presses! */
	monome_event_loop(monome);

	monome_close(monome);
	return 0;
}
