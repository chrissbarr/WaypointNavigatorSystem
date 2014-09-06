/*
 * waypointNavigator.cpp
 *
 * Author: Group 12
 * Group Members: Christopher Scott Barr - barcs003; Muhaini Harun - harmy045; Noor Fatin Amira Hasri - noofy001; Nur Faeza Zulkafli - zulny008, Stephen John Weatherley - weasj001; Megat Aiman Abu Bakar, axzqy008
 * 
 * Group Project: Waypoint Based Vehicle Control
 *
 * Project Overview: Designing a GPS guided flying vehicle that could carry a GoPro camera. 
 *
 */ 



//our included files
#include "main.h"		//stores constants and other stuff that multiple subsystems need to access
#include "timing.h"		//mash up of timing stuff borrowed from Arduino source files
#include "usart.h"
#include "debug.h"
#include "rx.h"

//other includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>

//-------------GPS INPUT FUNCTION PROTOTYPES---------------//
void gps_init();			//initializes GPS system, begins communication
bool gps_locked();			//checks if GPS is locked and data is available
void gps_update_values();	//gets data from GPS and stores relevant data in variables

//----------BAROMETER INPUT FUNCTION PROTOTYPES------------//
void baro_init();		//setup barometer and begin communication
float baro_get_reading();	//returns raw reading from barometer
float baro_get_height();	//calculates height from barometer value 

//-----------COMPASS INPUT FUNCTION PROTOTYPES-------------//
void compass_init();	//initializes compass module
float compass_get_heading();	//returns current heading, formatted as degrees relative to North (0-360)

//---------------WAYPOINT CORE PROTOTYPES---------------//
void waypoint_init_core();	//initializes arrays for long and lat, and sets waypoint index to zero
float waypoint_get_distance(float lat1, float long1, float lat2, float long2);	//returns the straight-line distance between two coordinates
float waypoint_get_angle(float lat1, float long1, float lat2, float long2);	//return an angle or heading from first pair to second pair of coordinates


int main(void)
{
	USART_init(USART_PC,9600);


    while(1)
    {
		debug_print("Testing debug functionality...");
		_delay_ms(5000);
    }
}
