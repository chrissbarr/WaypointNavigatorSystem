/*
 * waypoint.h
 *
 * Created: 8/09/2014 9:00:55 PM
 *  Author: Stephen John Weatherley
 */ 

#ifndef CoreHeader_H
#define CoreHeader_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_WAYPOINTS 100
#define NAME_STRING_LENGTH 5
#define UENTLAT 8
#define UENTLON 9
#define ULAT_TYPE 10
#define ULONG_TYPE 11

//define constants
#define PI 3.1415926535 //pi
#define EARTH_R 6371000 //Earth radius in metres
//#define EARTH_R_DIST 63.72795

// waypoint array
typedef struct {
	float latitude;
	float longitude;
	//float latitude_dist;
	//float longitude_dist;
	float latitude_deg;
	float longitude_deg;
} WayPoint;

// function prototypes
int user_waypoint_test (WayPoint[],int);

// initialises array for longitude and latitude & returns number of waypoints.
int userwaypoint(WayPoint[], int);

// prints waypoints
void printwaypoint(WayPoint [], int);

// function for degrees to radians
float deg_to_rad(float deg);

// function for radians to degrees
float rad_to_deg(float rad);

// function for wp difference
float difference(float num1, float num2);

//returns the straight-line distance between two coordinates
float waypoint_get_distance (float, float, float, float);

//returns bearing between two points
float waypoint_get_bearing(float, float, float, float);

// function to check if waypoint reached
int location_check (float, int);

// function to increment waypoint index
int index_check (int, int, int*);

// function to calculate percent to next
float check_percent (float, float, float, float, float);

// function to slow flight
int slow_flight (float, int);

//getter for external functions
float waypoint_get_heading();

#endif