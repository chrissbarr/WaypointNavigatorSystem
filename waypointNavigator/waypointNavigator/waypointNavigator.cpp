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

bool initialise();

//----------BAROMETER INPUT FUNCTION PROTOTYPES------------//
void baro_init();		//setup barometer and begin communication
float baro_get_reading();	//returns raw reading from barometer
float baro_get_height();	//calculates height from barometer value 

//-----------COMPASS INPUT FUNCTION PROTOTYPES-------------//
void compass_init();	//initializes compass module
float compass_get_heading();	//returns current heading, formatted as degrees relative to North (0-360)

float regulate_height(float);
float target_height;

int main(void)
{
	initialise();
	
	target_height = altimeter_get_start_height()+1.5;
	//arm_quad();
	
	while(1)
	{
		rx_update();
		if(rx_get_aux()>1200)
		{
			regulate_height(target_height);
			quad_output_passthrough(false,true,true,true,true,false);
		}
		else
		{
			quad_output_passthrough(true,true,true,true,true,false);
		}
		
		quad_output_set_aux(1000);	//keep flight controller in self level mode
		
	}
}

bool initialise()
{
	bool success = true;
	
	USART_init(USART_PC,9600);
	initTimers();
	rx_init();
	

	altimeter_init();
	quad_output_init();
	
	
	debug_println("Initialization complete!");
	
	return success;
}

float regulate_height(float targetHeight)
{
	float P = 1;
	float I = 0;
	float D = 0;
	
	float throttle = quad_output_get_throttle();
	
	float error = targetHeight - altimeter_get_metres();
	
	throttle += P*error;
	
	quad_output_set_throttle(throttle);
}

void arm_quad()
{
	quad_output_set_throttle(1000);
	quad_output_set_rudder(2000);
	_delay_ms(1000);
}