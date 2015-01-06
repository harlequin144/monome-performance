
//#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <monome.h>
#include "lo/lo.h"


//Constants
#define MONOME_DEVICE "osc.udp://127.0.0.1:13090/monocore"
#define MONOME_Y_LEN 8
#define MONOME_X_LEN 16
#define ON 0
#define OFF 1
#define TPB 32



/*
 * Global Variables
 */
monome_t *		monome;
uint16_t led_row[8] = { 0 };
uint16_t blinkers[8] = { 0 };

char quit = 0; 	//This means stop this program
char show	= ON;	

char tick = 0;

lo_address 		monocore_send; 



/* Led Functions */

void update_row( unsigned char i){
	monome_led_row(monome, 0, i, 2, (uint8_t *) &(led_row[i]));
}

void update_leds(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		update_row(i);
}

void set_led_on(char x, char y){			led_row[y] 	|= 1 << x; 	}
void set_led_off(char x, char y){			led_row[y] 	&= 1 << x;	}
void set_blinker_on(char x, char y){	blinkers[y] |= 1 << x;	}
void set_blinker_off(char x, char y){	blinkers[y] &= 1 << x;	}

void clear_leds(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		led_row[i] = 0;

	monome_led_all(monome, 0);
}

void clear_blinkers(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		blinkers[i] = 0;
}

void blink(){
	char i;
	for(i = 0; i < MONOME_Y_LEN; i++)
		led_row[i] ^= blinkers[i];

	update_leds();
}

/*
 * Function Prototypes
 */

// Osc Server Prototypes
void error(int num, const char *m, const char *path);

int	 quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 front_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);
int	 back_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

// Monome Function
void 	monome_press_handler(const monome_event_t *e, void *data);
void 	update_led();



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

void error(int num, const char *msg, const char *path){
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data){
    quit = 1;
    printf("quiting\n");
    fflush(stdout);
    return 0;
}

int tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data){
	tick = argv[0]->i;
	if(tick%16 == 0)
		blink();
	return 0;
}
int show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data){
	show = ON;
	monome_led_all(monome, 0);
	update_leds();
	return 0;
}


/**
 * this function gets registered with monome_register_handler
 * it gets called whenever a button is pressed
 */
void monome_press_handler(const monome_event_t *e, void *data) {
	unsigned int x, y;
	x = e->grid.x;
	y = e->grid.y;
	
	switch(x){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			break;
		case 14:
		case 15:
			if(y < 4)
				//switch the monome to qseq
				lo_send(monocore_send, "/show/qseq", NULL);
			//else
				//show = 2;
			break;
	}
	//printf("press %u, %u\n", x, y);
}



#define INIT_OSC_SERVER	\
  lo_server_thread_add_method(osc_server, "/quit", "", quit_handler, NULL);	\
  lo_server_thread_add_method(osc_server, "/tick", "i", tick_handler, NULL);	\
  lo_server_thread_start(osc_server);	


#define INIT_MONOME	\
	if( !(monome = monome_open(MONOME_DEVICE, "8002")) ){	\
		puts("Failed to open monome");	\
		return -1;	\
	}	\
	monome_led_all(monome, 0);	\
	monome_register_handler(monome, MONOME_BUTTON_DOWN, monome_press_handler,NULL);




int main() {
	//Exit if there is another instance...
  lo_server_thread osc_server = lo_server_thread_new("7711", error);
	INIT_OSC_SERVER

	monocore_send = lo_address_new_from_url("osc.udp://127.0.0.1:7710/monocore");

	//lo_send(pd_send, "/note", "i", 99);
	INIT_MONOME


	unsigned int i;
	for( i = 0; i < 8; i++ ) {
		if(i < 4){
			set_led_on(15,i);
			set_led_on(14,i);
		}
		set_blinker_on(14,i);
		set_blinker_on(15,i);
	}
	update_leds();



	// Main loop
	while(!quit) {
		while( monome_event_handle_next(monome) );

		chill(50);
	}


	// Clean up.
	monome_led_all(monome, 0);
	monome_close(monome);
  lo_server_thread_free(osc_server);
	return 0;
}
