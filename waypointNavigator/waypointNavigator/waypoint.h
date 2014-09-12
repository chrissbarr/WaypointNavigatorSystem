/*
 * waypoint.h
 *
 * Created: 8/09/2014 9:00:55 PM
 *  Author: Stephen John Weatherley
 */ 

#include <math.h>
#include <stdio.h>
#include "gps.h"

//define constants
#define PI 3.1415926535		//pi
#define EARTH_R  6371000	//Earth radius in metres




//---------------WAYPOINT CORE PROTOTYPES---------------//
void waypoint_init_core();	//initializes arrays for long and lat, and sets waypoint index to zero
float waypoint_get_distance(float lat1, float long1, float lat2, float long2);	//returns the straight-line distance between two coordinates
float waypoint_get_bearing(float lat1, float long1, float lat2, float long2);	//return an angle or heading from first pair to second pair of coordinates


//tools we need
double deg_to_rad(double deg);
double rad_to_deg(double rad);
double difference(double,double);
