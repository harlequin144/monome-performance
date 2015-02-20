#include <assert.h>
#define RUN_TESTS 0


void test_calculate_tick_period();

void test_double_to_timespec();
void test_timespec_to_double();
void test_timespec_norm();

void test_timespec_geq();
void test_timespec_leq();
void test_period_to_bpm();
void test_bpm_to_period();






void run_tests()
{
	if( RUN_TESTS == 1 ){
		puts("running tests!");

		puts("testing timespec norm");
		test_timespec_norm();

		puts("testing double_to_timespec");
		test_double_to_timespec();

		puts("testing timespec_to_double");
		test_timespec_to_double();

		puts("testing calculate_tick_period");
		test_calculate_tick_period();

		printf("\n\nTesting complete!\n\n");
	}
}




void test_timespec_norm()
{
	struct timespec t1 = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec t2 = {.tv_sec = 0, .tv_nsec = 20000};
	struct timespec t3 = {.tv_sec = 1, .tv_nsec = 31234};
	struct timespec t4 = {.tv_sec = 3, .tv_nsec = 64567};

	struct timespec result = timespec_norm( t1, t2 );
	assert( result.tv_sec == 0 );
	assert( result.tv_nsec == 20000 );

	result = timespec_norm( t2, t1 );
	assert( result.tv_sec == 0 );
	assert( result.tv_nsec == 20000 );

	result = timespec_norm( t1, t3 );
	assert( result.tv_sec == 1 );
	assert( result.tv_nsec == 31234 );

	result = timespec_norm( t3, t1 );
	assert( result.tv_sec == 1 );
	assert( result.tv_nsec == 31234 );

	result = timespec_norm( t2, t3 );
	assert( result.tv_sec == 1 );
	assert( result.tv_nsec == 11234 );

	result = timespec_norm( t3, t2 );
	assert( result.tv_sec == 1 );
	assert( result.tv_nsec == 11234 );

	result = timespec_norm( t2, t1 );
	assert( result.tv_sec == 0 );
	assert( result.tv_nsec == 20000 );

	result = timespec_norm( t3, t4 );
	assert( result.tv_sec ==  2);
	assert( result.tv_nsec == 33333 );
}




void test_calculate_tick_period()
{
	struct transport trans;
	struct transport_params params;
	strcpy(params.transport_port,"8000");
	//params.monome_port = "";
	//params.bridge_port = "";
	params.tick_clients = NULL;
	params.bpm_clients = NULL;
	
	puts("here");
	new_transport( &trans, &params);

	struct timespec t1 = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec t2 = {.tv_sec = 0, .tv_nsec = 20000};
	struct timespec t3 = {.tv_sec = 1, .tv_nsec = 31234};
	struct timespec t4 = {.tv_sec = 1, .tv_nsec = 64567};


	puts("tapping t1");
	tap( &trans, t1 );
	printf("\n");
	puts("tapping t2");
	tap( &trans, t2 );
	printf("\n");
	puts("tapping t3");
	tap( &trans, t3 );
	printf("\n");
	puts("tapping t4");
	tap( &trans, t4 );
	printf("\n");

	calculate_tick_period( &trans );
	puts("final tick period");
	print_timespec( trans.tick_period );
	assert( trans.tick_period.tv_sec == 0 );
	assert( trans.tick_period.tv_nsec == 2314964 );


}

void test_double_to_timespec(){
	struct timespec r1 = double_to_timespec(1.23456);
	struct timespec r2 = double_to_timespec(0);
	struct timespec r3 = double_to_timespec(1);
	struct timespec r4 = double_to_timespec(1.000000001);
	struct timespec r5 = double_to_timespec(2.0000000001);
	struct timespec r6 = double_to_timespec(3.00000000001);
	struct timespec r7 = double_to_timespec(-1.0000000001);
	struct timespec r8 = double_to_timespec(1.2345693534638539);
	struct timespec r9 = double_to_timespec(1.9999999998999999999999999);

	assert( r1.tv_sec == 1 );
	assert( r1.tv_nsec == 234560000 );
	assert( r2.tv_sec == 0 );
	assert( r2.tv_nsec == 0 );
	assert( r3.tv_sec == 1 );
	assert( r3.tv_nsec == 0 );
	assert( r4.tv_sec == 1 );
	assert( r4.tv_nsec == 1 );
	assert( r5.tv_sec == 2 );
	assert( r5.tv_nsec == 0 );
	assert( r6.tv_sec == 3 );
	assert( r6.tv_nsec == 0 );
	assert( r7.tv_sec == 1 );
	assert( r7.tv_nsec == 0 );
	assert( r8.tv_sec == 1 );
	assert( r8.tv_nsec == 234569353);
	//assert( r9.tv_sec == 1 );
	//assert( r9.tv_nsec == 999999999);

	assert( r1.tv_sec >= 0); assert( r1.tv_nsec >= 0);
	assert( r2.tv_sec >= 0); assert( r2.tv_nsec >= 0);
	assert( r3.tv_sec >= 0); assert( r3.tv_nsec >= 0);
	assert( r4.tv_sec >= 0); assert( r4.tv_nsec >= 0);
	assert( r5.tv_sec >= 0); assert( r5.tv_nsec >= 0);
	assert( r6.tv_sec >= 0); assert( r6.tv_nsec >= 0);
	assert( r7.tv_sec >= 0); assert( r7.tv_nsec >= 0);
	assert( r8.tv_sec >= 0); assert( r8.tv_nsec >= 0);
	assert( r9.tv_sec >= 0); assert( r9.tv_nsec >= 0);

	assert( r1.tv_nsec <= NANOS_PER_SEC);
	assert( r2.tv_nsec <= NANOS_PER_SEC);
	assert( r3.tv_nsec <= NANOS_PER_SEC);
	assert( r4.tv_nsec <= NANOS_PER_SEC); 
	assert( r5.tv_nsec <= NANOS_PER_SEC);
	assert( r6.tv_nsec <= NANOS_PER_SEC);
	assert( r7.tv_nsec <= NANOS_PER_SEC);
	assert( r8.tv_nsec <= NANOS_PER_SEC);
	assert( r9.tv_nsec <= NANOS_PER_SEC);

}

void test_timespec_to_double()
{
	struct timespec t1 = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec t2 = {.tv_sec = 0, .tv_nsec = 20000};
	struct timespec t3 = {.tv_sec = 1, .tv_nsec = 31234};
	struct timespec t4 = {.tv_sec = 1, .tv_nsec = 64567};
	struct timespec t5 = {.tv_sec = 0, .tv_nsec = 1000000000};
	struct timespec t6 = {.tv_sec = 0, .tv_nsec = 5000000000};
	struct timespec t7 = {.tv_sec = 0, .tv_nsec = 50000000000};

	double r1 = timespec_to_double(t1);
	double r2 = timespec_to_double(t2);
	double r3 = timespec_to_double(t3);
	double r4 = timespec_to_double(t4);
	double r5 = timespec_to_double(t5);
	double r6 = timespec_to_double(t6);
	double r7 = timespec_to_double(t7);

	assert( r1 == 0.0);
	//printf("%0.10f\n", r2);
	assert( r2 == 0.00002 );
	assert( r3 == 1.000031234 );
	assert( r4 == 1.000064567 );
	assert( r5 == 1.0 );
	assert( r6 == 5.0 );
	assert( r7 == 50.0 );

}

//void test_timespec_geq();
//void test_timespec_leq();
//void test_period_to_bpm();
//void test_bpm_to_period();
