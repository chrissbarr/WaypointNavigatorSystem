/*
 * gps.h
 *
 * Created: 12/09/2014 9:22:37 AM
 *  Author: Chris Barr
 */ 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "main.h"
#include "debug.h"
#include "usart.h"

#define HEADER_TYPE 5
#define TIME_TYPE 9
#define LAT_TYPE 10
#define LONG_TYPE 11
#define LAT_HEADINGCHAR 2
#define LONG_HEADINGCHAR 2
#define SAT_TYPE 2
#define COURSE_TYPE 6
#define UNUSED 47
#define COURSE 7

//-------------GPS INPUT FUNCTION PROTOTYPES---------------//
bool gps_init();			//initializes GPS system, begins communication
bool gps_locked();			//checks if GPS is locked and data is available
void gps_update_values();	//gets data from GPS and stores relevant data in variables

float gps_conv_coords(float in_coords);

float gps_time_func(void);
float gps_get_lat(void);
float gps_get_long(void);
float gps_get_sats(void);
float gps_get_course(void);