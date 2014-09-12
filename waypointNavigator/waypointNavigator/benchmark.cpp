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
	float time_start;
	int loops = 100;
	float test_lat[loops+1];
	float test_long[loops+1];
	
	float time_start_gen_array = millis();
	
	for(i = 0; i < loops+1; i++)
	{
		test_lat[i]=3442.66+i/100;
		test_long[i]=13842.66+i/100;
	}
	
	float time_gen_array = millis()-time_start_gen_array;
	
	time_start = millis();
	
	for(i = 0; i < loops; i++)
	{
		waypoint_get_distance(test_lat[i],test_long[i],test_lat[i+1],test_long[i+1]);
	}
	
	float time_waypoint = millis()-time_start;
	
	float time_per_operation = time_waypoint / loops;
	
	debug_print("Time to generate coordinates:\n");
	debug_printf(time_gen_array);
	
	debug_print("Time to complete loops:\n");
	debug_printf(time_waypoint);
	
	debug_print("Average time per waypoint_get_distance operation:\n");
	debug_printf(time_per_operation);
	
	debug_print("End benchmark");
	
	
}

void benchmark_gps_conv_coords()
{
	debug_print("Coordinates in DDMM.MMMM format:");
	float coordinate_in_ddmm_lat = 3449.040;
	float coordinate_in_ddmm_long = 13842.678;

	debug_printf(coordinate_in_ddmm_lat);
	debug_printf(coordinate_in_ddmm_long);

	debug_print("Coordinates in DD.DDDD format:");
	float lat_in_dec_degrees = gps_conv_coords(coordinate_in_ddmm_lat);
	float long_in_dec_degrees = gps_conv_coords(coordinate_in_ddmm_long);

	debug_printf(lat_in_dec_degrees);
	debug_printf(long_in_dec_degrees);
}

void benchmark_millis()
{
	debug_print("Counting 1s ten times...");
	for(int i = 0; i < 10; i++)
	{
		float time=millis();
		_delay_ms(1000);
		float time_final = millis()-time;
		
		debug_printf(time_final);
	}
	debug_print("Finished Counting!");
}

void benchmark_micros()
{
	debug_print("Counting 1s ten times... (micros)");
	for(int i = 0; i < 10; i++)
	{
		float time=micros();
		_delay_ms(1000);
		float time_final = micros()-time;
		
		debug_printf(time_final);
	}
	debug_print("Finished Counting!");
}