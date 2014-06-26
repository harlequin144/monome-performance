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

int	 tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);

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
				((struct Dtrig*) user_data)->show);

		printf("\n");
		fflush(stdout);
    return 0;		
}

int 
quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{		
		((struct Dtrig*) user_data)->quit = 1;
    return 0;
}

int 
show_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		struct Dtrig* dtrig = ((struct Dtrig*) user_data);

		// State update
		dtrig->show = 1;
		dtrig->trans_press_cnt = 0;
		
		// Redraw leds
			light_dtrig(dtrig);

    return 0;
}

int 
stop_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		struct Dtrig* dtrig = ((struct Dtrig*) user_data);
		dtrig->trans = 0;
		lo_send(dtrig->bridge_send, "/dtrig/grid/led/map", "iiiiiiiiii", 0, 0,
			0,0,0,0, 0,0,0,1);
}

int 
hide_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
		struct Dtrig* dtrig = ((struct Dtrig*) user_data);
		dtrig->show = 0;
		int i;
		for(i = 0; i < 16; i++)
			dtrig->trig_press_cnt[i] = 0;
		dtrig->trans_press_cnt = 0;
    return 0;
}


int 
tick_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	// For convenience
	struct Dtrig* dtrig = ((struct Dtrig*) user_data);
	char new_tick = (argv[0]->i)%TICKS_PER_BEAT;

	if(dtrig->trans == 0) 
		dtrig->trans = dtrig->trans + 1;

	// mod 3is like 32 note quant
	// mod 6is like 16 note quant
	if(new_tick%12 == 0){
		if(dtrig->trig_mask != 0){
		int i;
		for(i = 0; i < 12; i++){
			if((dtrig->trig_mask & ONE_SHIFT(i)) != 0)
				lo_send(dtrig->sc_send, "/sc/dserver/trigger", "i", i);
		}

		dtrig->trig_mask = 0;
		}
	}
	

	if(new_tick == 0)
		// down beat
		lo_send(dtrig->bridge_send, "/dtrig/grid/led/map", "iiiiiiiiii", 0, 0,
			63,56,42,56, 0,0,0,1);

	else if(new_tick == 24)
		lo_send(dtrig->bridge_send, "/dtrig/grid/led/map", "iiiiiiiiii", 0, 0,
			63,7,21,7, 0,0,0,1);

	return 0;
} // tick_handler



/*
 * Press handler helpers
 */

void control_press(struct Dtrig* dtrig, short x, short y){
	if(y == 0){
		if(dtrig->trans == 0)
			lo_send(dtrig->sc_send, "/sc/transport/start", NULL);
		else
			lo_send(dtrig->sc_send, "/sc/transport/stop", NULL);
	}
	else if(y > 0 && y < 4){
		dtrig->trans_press_cnt = dtrig->trans_press_cnt +1;
		if(dtrig->trans_press_cnt == 1)

			if(x < 3)	//clear tap
				lo_send(dtrig->sc_send, "/sc/transport/clear_tap", NULL);
			
			else if(x >= 3 && x < 6)	//tap
				lo_send(dtrig->sc_send, "/sc/transport/tap", NULL);

	} 
	else if(y ==7){
		if(x == 0){
			lo_send(dtrig->bridge_send, "/dtrig/hide", NULL);
			int i;
			for(i = 0; i < 16; i++)
				dtrig->trig_press_cnt[i] = 0;
			dtrig->trans_press_cnt = 0;
		}
	}

}

void control_release(struct Dtrig* dtrig, short x, short y){
	if(y > 0 && y < 4){
		dtrig->trans_press_cnt = dtrig->trans_press_cnt -1;
		if(dtrig->trans_press_cnt < 0)
			dtrig->trans_press_cnt = 0;
	} 

}

void trigger_press(struct Dtrig* dtrig, char x, char y){
	char trigger = ((x-8)/2)*4 + (y/2);

	dtrig->trig_press_cnt[trigger] = dtrig->trig_press_cnt[trigger] +1;

	if( dtrig->trig_press_cnt[trigger] == 1){
		if(dtrig->trans == 0)
			lo_send(dtrig->sc_send, "/sc/dserver/trigger", "i", 
					dtrig->trig_dn_drum_num[trigger]);

		else{
			printf("trig mask before: %u\n", dtrig->trig_mask );
			dtrig->trig_mask |= ONE_SHIFT(dtrig->trig_dn_drum_num[trigger]);
			printf("trig mask after: %u\n", dtrig->trig_mask );
		}
	}
}

void trigger_release(struct Dtrig* dtrig, char x, char y){
	char trigger = ((x-8)/2)*4 + (y/2);

	dtrig->trig_press_cnt[trigger] = dtrig->trig_press_cnt[trigger] -1;
	if( dtrig->trig_press_cnt[trigger] < 0)
		dtrig->trig_press_cnt[trigger] = 0;

	if( dtrig->trig_press_cnt[trigger] == 0){
		//if enum type
		if(dtrig->trig_up_drum_num[trigger] >= 0){
			if(dtrig->trans == 0)
				lo_send(dtrig->sc_send, "/sc/dserver/trigger", "i", 
					dtrig->trig_up_drum_num[trigger]);
			else
				dtrig->trig_mask |= ONE_SHIFT(dtrig->trig_up_drum_num[trigger]);
		}
	}

}

int 
press_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
	char x = argv[0]->i;
	char y = argv[1]->i;
	char v = argv[2]->i;			
	//printf("Press %u, %u\n", x, y);
	
	struct Dtrig* dtrig = ((struct Dtrig*) user_data);

	// Press
	if(v == 1){
		if(x < 8)
			control_press(dtrig, x, y);
		else
			trigger_press(dtrig, x, y);
	}
	// Release
	else{
		if(x < 8)
			control_release(dtrig, x, y);
		else
			trigger_release(dtrig, x, y);
	}

	return 0;
}


