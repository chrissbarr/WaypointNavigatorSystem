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
#include "waypoint.h"
#include "benchmark.h"
//#include "gps.h"
#include "quad_output.h"
#include "altimeter.h"

//other includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>

void initialise();

//----------BAROMETER INPUT FUNCTION PROTOTYPES------------//
void baro_init();		//setup barometer and begin communication
float baro_get_reading();	//returns raw reading from barometer
float baro_get_height();	//calculates height from barometer value 

//-----------COMPASS INPUT FUNCTION PROTOTYPES-------------//
void compass_init();	//initializes compass module
float compass_get_heading();	//returns current heading, formatted as degrees relative to North (0-360)


int main(void)
{
	initialise();
	_delay_ms(1000);
	debug_printf(mpl_getAlt(altimeter_get_status()));
	debug_print("\n\r");
    while(1)
    {
		_delay_ms(2000);
		debug_printf(mpl_getAlt(altimeter_get_status()));
		debug_println("Looping...");
		//rx_update();
		//quad_output_passthrough(true,true,true,true,true,true);
    }
}

void initialise()
{
	USART_init(USART_PC,9600);
	initTimers();
	//gps_init();
	//rx_init();
	//quad_output_init();
	altimeter_init();
	
	debug_println("Initialization complete!");
}