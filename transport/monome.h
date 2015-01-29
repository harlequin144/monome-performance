#include <lo/lo.h>

#define LED_MASK_DOWN 255, 15, 105, 105, 15, 66, 102, 102
#define LED_MASK_UP 	255, 240, 150, 150, 240, 66, 102, 102
#define LED_MASK_STOP	0, 0, 0, 0, 0, 66, 102, 102
#define LED_MASK_CLEAR 0, 0, 0, 0, 0, 0, 0, 0


//struct monome{
//
//	int run;
//	int show;
//
//	int press_count;
//
//	int bpm_mask[8];
//
//	lo_server monome_osc_server;
//	lo_address bridge_address; 
//	lo_address transport_address; 
//};


void * start_monome( void *ptr );

struct monome * 
new_monome( struct monome * mono, char * monome_port, char * transport_port,
		char * bridge_port );

void * start_monome_loop( struct monome * mono );

int monome_quit_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int show_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int hide_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int set_monome_bpm_handler(const char * path, const char * types, lo_arg **
		argv, int argc, void * data, void * user_data );
int tick_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int monome_stop_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int press_handler(const char * path, const char * types, lo_arg ** argv,
		int argc, void * data, void * user_data );
int monome_generic_handler(const char * path, const char * types, lo_arg **
		argv, int argc, void * data, void * user_data );

