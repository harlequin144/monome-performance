#include <time.h>
#include <unistd.h>

#include "transport.h"
 


void 	timespec_print(struct timespec time);
int 	timespec_geq(struct timespec time1, struct timespec time2);
int 	timespec_leq(struct timespec time1, struct timespec time2);
struct timespec timespec_norm(struct timespec time1, struct timespec time2);
int 	check_tick_expired(struct timespec elapsed, struct timespec period);

int 	validate_bpm(float bpm);
int 	validate_period(struct timespec period);

float period_to_bpm(struct timespec period);
struct timespec bpm_to_period(float bpm);




void timespec_print(struct timespec time)
{
	printf("\t %li \n\t %li\n", time.tv_sec, time.tv_nsec);
}

int timespec_geq(struct timespec time1, struct timespec time2)
{
	if( (time1.tv_sec >= time2.tv_sec) && (time1.tv_nsec >= time2.tv_nsec) )
		return 1;
	else
		return 0;
}

int timespec_leq(struct timespec time1, struct timespec time2)
{
	if( (time1.tv_sec <= time2.tv_sec) && (time1.tv_nsec <= time2.tv_nsec) )
		return 1;
	else
		return 0;
}


int check_tick_expired(struct timespec elapsed, struct timespec period)
{
	if( timespec_geq(elapsed, period) ){
		return 1; //puts("late");
	}

	else{
		struct timespec delta;
		delta = timespec_norm(elapsed, period);
		if( (delta.tv_sec == 0) && (delta.tv_nsec <= TOL) ){
			return 1; //puts("early");
		}
	}

	return 0;
}


struct timespec timespec_norm(struct timespec time1, struct timespec time2)
{
	struct  timespec  result;
	result.tv_nsec = result.tv_nsec = 0 ;

	if(time1.tv_sec == time2.tv_sec){
		result.tv_sec = 0;
		result.tv_nsec = abs((time1.tv_nsec - time2.tv_nsec));
	}
	else
		
	if(time1.tv_sec < time2.tv_sec){		
		result.tv_nsec = NANOS_PER_SEC - time1.tv_nsec;
		time1.tv_sec += 1;

		result.tv_nsec += time2.tv_nsec;

		result.tv_sec = abs(time1.tv_sec - time2.tv_sec);
		result.tv_sec += result.tv_nsec / NANOS_PER_SEC;
		result.tv_nsec = result.tv_nsec % NANOS_PER_SEC;
	} 
	else {						
		result =  timespec_norm(time2, time1);
	}
	
	return result;
}

float period_to_bpm(struct timespec period)
{
	float sec_per_tick = period.tv_sec;
	sec_per_tick += ((float) period.tv_nsec / NANOS_PER_SEC);

	float tick_per_sec = 1 / sec_per_tick;
	float beat_per_sec = tick_per_sec / TICKS_PER_BEAT;
	float bpm = beat_per_sec * 60;

	return bpm;
}
	
struct timespec bpm_to_period(float bpm)
{
	//need some security checking here.
	assert(bpm <= 400);
	assert(bpm >= 0);

	float bps = bpm / 60.0;
	float ticks_per_sec = bps * TICKS_PER_BEAT;
	float sec_per_tick = 1.0 / ticks_per_sec;
	long long sec = (long long) sec_per_tick;
	long long nsec = (sec_per_tick - ((long long) sec_per_tick)) * NANOS_PER_SEC;

	struct timespec ret;
	ret.tv_sec = sec;
	ret.tv_nsec = nsec;
	return ret;
}


int validate_bpm(float bpm)
{
	return validate_period( bpm_to_period(bpm) );
}


int validate_period(struct timespec period)
{
	if( timespec_geq(period, MIN_PERIOD) && timespec_leq(period, MAX_PERIOD) )
		return 1;
	else
		return 0;
}
