#include <time.h>
#include <unistd.h>

#include "transport.h"
 



int check_tick_expired(struct transport * trans){
	struct timespec current_time;
	struct timespec elapsed_time;

	int ret = 0;
	ret = clock_gettime(CLOCK_MONOTONIC, &current_time);
	if(ret < 0)
		return ret;

	elapsed_time = timespec_norm(trans->last_tick_time, current_time);

	if( (elapsed_time.tv_sec >= trans->tick_period.tv_sec) && 
			(elapsed_time.tv_nsec >= trans->tick_period.tv_nsec) )
	{
		trans->last_tick_time = current_time;
		return 1;
		//puts("late");
	}

	else{
		struct timespec delta;
		delta = timespec_norm(elapsed_time, trans->tick_period);
		if((delta.tv_sec == 0) && (delta.tv_nsec <= TOL)){
			trans->last_tick_time = current_time;
			//puts("early");
			//printf("elapsed: \n\t %li \n\t %li\n", norm2.tv_sec, norm2.tv_nsec);
			return 1;
		}
	}

	return 0;
}


void print_timespec(struct timespec time)
{
	printf("\t %li \n\t %li\n", time.tv_sec, time.tv_nsec);
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
	int ret = 1;
	if((bpm < 20) || (bpm > 400))
		ret = 0;

	return ret;
}

int validate_period(struct timespec period)
{
	float bpm = period_to_bpm(period);
	return validate_bpm(bpm);
}
