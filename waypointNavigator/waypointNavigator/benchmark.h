/*
 * benchmark.h
 *
 * Created: 9/09/2014 10:44:27 AM
 *  Author: Chris Barr
 */ 

#include "waypoint.h"
#include "timing.h"
#include "debug.h"
#include <util/delay.h>

void benchmark_waypoint_get_distance();
void benchmark_gps_conv_coords();
void benchmark_millis();
void benchmark_micros();