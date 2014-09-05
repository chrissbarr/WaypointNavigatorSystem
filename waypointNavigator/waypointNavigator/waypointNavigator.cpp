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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>


	///////////////INPUT FUNCTIONS/////////////////////
//-------------GPS INPUT FUNCTION PROTOTYPES---------------//
void init_gps();			//initializes GPS system, begins communication
bool gps_locked();			//checks if GPS is locked and data is available
void gps_update_values();	//gets data from GPS and stores relevant data in variables

//----------BAROMETER INPUT FUNCTION PROTOTYPES------------//
void init_baro();		//setup barometer and begin communication
float get_baro_reading();	//returns raw reading from barometer
float get_baro_height();	//calculates height from barometer value 

//-----------COMPASS INPUT FUNCTION PROTOTYPES-------------//
void init_compass();	//initializes compass module
float get_compass_heading();	//returns current heading, formatted as degrees relative to North (0-360)

//--------------RX INPUT FUNCTION PROTOTYPES---------------//
void init_rx_input();

//-------------RX Input Variables-----------------------//


#define THROTTLE_IN_PIN PB0		//Which RX channels map to which pins in port
#define AILERON_IN_PIN PB1
#define ELEVATOR_IN_PIN PB2
#define RUDDER_IN_PIN PB3
#define GEAR_IN_PIN PB4
#define AUX_IN_PIN PB5

//flags to represent changes in different channels
#define THROTTLE_FLAG 1		
#define AILERON_FLAG 2
#define ELEVATOR_FLAG 4
#define RUDDER_FLAG 8
#define GEAR_FLAG 16
#define AUX_FLAG 32

//shared variables used in interrupts
volatile uint16_t unThrottleInShared;
volatile uint8_t rxUpdateFlagsShared;

//variables used outside of interrupts (non-volatile)
uint32_t ulThrottleStart;
static uint8_t rxUpdateFlags;


/*
//Pin Change Interrupt for reading Throttle Value//
ISR(PCINT0_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(THROTTLE_IN_PIN))
	{
		//ulThrottleStart = micros();
	}
	else
	{
		//unThrottleInShared = (uint16_t)(micros()-ulThrottleStart);
		rxUpdateFlagsShared |= THROTTLE_FLAG;
	}
	
}
*/

//---------------WAYPOINT CORE PROTOTYPES---------------//
void init_waypoint_core();	//initializes arrays for long and lat, and sets waypoint index to zero
float waypoint_get_distance(float lat1, float long1, float lat2, float long2);	//returns the straight-line distance between two coordinates
float waypoint_get_angle(float lat1, float long1, float lat2, float long2);	//return an angle or heading from first pair to second pair of coordinates


int main(void)
{
	USART_init();


    while(1)
    {
		USART_putstring("test");    //Pass the string to the USART_putstring function and sends it over the serial
		_delay_ms(5000);
    }
}
