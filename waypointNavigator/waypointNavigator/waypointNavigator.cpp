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
void beep();
float target_height;
float current_time = 0;
float previous_time = 0;
float integral = 0;

int main(void)
{
	initialise();
	
	target_height = altimeter_get_start_height()+2;
	//arm_quad();
	
	while(1)
	{
		rx_update();
		if(rx_get_aux()>1300 && rx_get_aux()<1600)
		{
			//debug_println("regulating height!");
			regulate_height(target_height);
			quad_output_passthrough(false,true,true,true,true,false);
			
			/*
			if(rx_get_throttle()<1300)
			{
				target_height-=.05;
			}
			if(rx_get_throttle()>1700)
			{
				target_height+=.05;
			}*/
			/*
			if(target_height<altimeter_get_start_height())
				target_height=altimeter_get_start_height();
			*/
		}
		else if(rx_get_aux()>1600)
		{
			target_height = altimeter_get_metres();
		}
		else
		{
			debug_println("Not regulating height!");
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
	

	if(altimeter_init()==false)
	{
		while(1)
		{
			beep();
			beep();
			beep();
			beep();
			_delay_ms(200);
		}
	}
	quad_output_init();
	
	
	debug_println("Initialization complete!");
	beep();
	beep();
	beep();
	return success;
}

float regulate_height(float targetHeight)
{
	float dt = (current_time-previous_time)/1000;
	previous_time = current_time;
	current_time = millis();
	float P = 10;
	float I = 0;
	float D = 0;
	
	float currentHeight = altimeter_get_metres();
	debug_print("Target height: ");
	debug_printf(targetHeight);
	debug_print("m\t Current height: ");
	debug_printf(currentHeight);
	debug_println("m.");
	float throttle = quad_output_get_throttle();
	
	float error = (targetHeight - currentHeight)*dt;
	
	integral += (error);
	
	if(error<0)
		beep();
		
	throttle += (P*error)+integral*I;
	
	if(throttle>1600)
		throttle=1600;
	
	quad_output_set_throttle(throttle);
}

void arm_quad()
{
	quad_output_set_throttle(1000);
	quad_output_set_rudder(2000);
	_delay_ms(1000);
}

void beep()
{
	sbi(PORTB,7);
	_delay_ms(50);
	cbi(PORTB,7);
}
