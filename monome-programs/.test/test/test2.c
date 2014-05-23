/**
 * simple.c
 * press a button to toggle it!
 */

#include <stdlib.h>
#include <stdio.h>
#include <monome.h>

unsigned int grid[16][16] = { [0 ... 15][0 ... 15] = 0 };

#define MONOME_DEVICE "osc.udp://127.0.0.1:13090/bridge"

/**
 * this function gets registered with monome_register_handler
 * it gets called whenever a button is pressed
 */
void handle_press(const monome_event_t *e, void *data) {
	unsigned int x, y;

	puts("presshandle");

	x = e->grid.x;
	y = e->grid.y;

	/* toggle the button */
	grid[x][y] = !grid[x][y];
	monome_led_set(e->monome, x, y, grid[x][y]);
}

int main(int argc, char *argv[]) {
	monome_t *monome;

	//puts("program running");

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
