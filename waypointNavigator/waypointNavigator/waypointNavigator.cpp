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
#include "gps.h"
#include "quad_output.h"
#include "altimeter.h"
#include "quad_control.h"
#include "compass.h"
#include "accelerometer.h"

//other includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>

bool gps_demonstration = false;

bool initialise();

int main(void)
{
	initialise();
	uint16_t last_changed = millis();
	
	while(1)
	{

		if(gps_demonstration==true)
		{
			if(gps_update())
			{
					chris_waypoint_update();
					debug_print("H");
					debug_print(",");
					debug_printf(gps_get_latitude());
					debug_print(",");
					debug_printf(gps_get_longitude());
					debug_print(",");
					debug_printi(gps_get_sats());
					debug_print(",");
					debug_printf(altimeter_get_metres());
					debug_print(",");
					debug_printf(chris_waypoint_current_bearing());	//target heading
					debug_print(",");
					float current_heading = compass_get_heading();
					debug_printf(current_heading);
					debug_print(",");
					debug_printf(current_heading-chris_waypoint_current_bearing());
					debug_print(",");
					debug_printi(chris_waypoint_current_index());
					debug_print(",");
					debug_printf(chris_waypoint_current_distance());
					debug_print(",");
					debug_printf(array_get_lat(chris_waypoint_current_index()));
					debug_print(",");
					debug_printf(array_get_lon(chris_waypoint_current_index()));
					debug_println(",");
				}
		
				//automatic testing, every 3 seconds change to the next waypoint
				if(millis()-last_changed>3000 && 1==2)
				{
					chris_set_waypoint_current_index(chris_waypoint_current_index()+1);
					last_changed=millis();
				}
		
				if(chris_waypoint_current_index()>chris_get_max_waypoints())
				chris_set_waypoint_current_index(0);
			}
			else
			{
				//demonstrate reading receiver
				rx_update();
				quad_output_passthrough(true,true,true,true,true,true);
			}
	}
}

bool initialise()
{
	
	bool success = true;

	
	USART_init(USART_PC,9600);
	debug_println("Begininning Initialisation...");
	
	initTimers();
	
	if(gps_demonstration==true)
	{
		//GPS Demonstration
		altimeter_init();
		
		init_HMC5883L();
		
		
		gps_init();
		
		//Initialise a series of waypoints in a circle around the current coordinates..
		_delay_ms(1000);
		chris_waypoint_init();
	}
	else
	{
		//RX and servo Demonstration
		rx_init();
		quad_output_init();
	}
	
	
	debug_println("Initialization succeeded!");
	
	//beep some pattern I can recognize
	debug_beep_long();
	_delay_ms(250);
	debug_beep();
	_delay_ms(250);
	debug_beep();
	_delay_ms(250);
	debug_beep();
	_delay_ms(250);
	debug_beep_long();
	
	return success;
}

