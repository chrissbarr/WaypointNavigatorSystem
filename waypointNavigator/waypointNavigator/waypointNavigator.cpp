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

//other includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>

bool initialise();

int main(void)
{
	initialise();
	
	
	while(1)
	{
		rx_update();
		
		
	}
}

bool initialise()
{
	bool success = true;
	
	USART_init(USART_PC,9600);
	initTimers();
	rx_init();
	
	if(altimeter_init()==false)
		success=false;

	quad_output_init();
	
	if(success==false)
	{
		debug_println("Initialization failed!");
		//if something is broken, we can't proceed
		while(1)
		{
			debug_beep_long();
			_delay_ms(200);
		}
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

