/*
 * waypointNavigator.cpp
 *
 * Created: 29/08/2014 09:23:13
 *  Author: Group 12
 */ 

#define F_CPU  16000000L		//16MHz clock rate

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "timing.h"		//mash up of timing stuff borrowed from Arduino source files


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
#define RX_PORT PINB	//port RX is connected to

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


//Pin Change Interrupt for reading Throttle Value//
ISR(PCINT0_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(THROTTLE_IN_PIN))
	{
		ulThrottleStart = micros();
	}
	else
	{
		unThrottleInShared = (uint16_t)(micros()-ulThrottleStart);
		rxUpdateFlagsShared |= THROTTLE_FLAG;
	}
	
}

int main(void)
{
	init_rx_input();

    while(1)
    {
        //TODO:: Please write your application code 
    }
}

void init_rx_input()
{
	//Enable Pin Change Interrupts
		//todo
		
	initTimers();
} 