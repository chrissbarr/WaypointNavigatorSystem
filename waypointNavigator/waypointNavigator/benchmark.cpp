/*
 * benchmark.cpp
 *
 * Created: 9/09/2014 10:43:42 AM
 *  Author: Chris Barr
 * Used to test the speed of various functions
 */ 

#include "benchmark.h"


void benchmark_waypoint_get_distance()
{
	int i;
	int time_start;
	int loops = 100;
	float test_lat[loops];
	float test_long[loops];
	
	int time_start_gen_array = millis();
	
	for(i = 0; i < loops+1; i++)
	{
		test_lat[i]=3442.66227+i/100;
		test_long[i]=13842.66827+i/100;
	}
	
	int time_gen_array = millis()-time_start_gen_array;
	
	time_start = millis();
	
	for(i = 0; i < loops; i++)
	{
		waypoint_get_distance(test_lat[i],test_long[i],test_lat[i+1],test_long[i+1]);
	}
	
	int time_waypoint = millis()-time_start;
	
	float time_per_operation = time_waypoint / loops;
	
	debug_print("Time to generate coordinates:\n");
	debug_print(time_gen_array);
	debug_print("Time to complete loops:\n");
	debug_print(time_waypoint);
	debug_print("Average time per waypoint_get_distance operation:\n");
	debug_print(time_per_operation);
}