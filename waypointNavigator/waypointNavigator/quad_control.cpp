/*
 * quad_control.cpp
 *
 * Created: 7/10/2014 5:46:17 PM
 *  Author: Chris
 */ 
#include "quad_control.h"

float target_height;
float current_time = 0;
float previous_time = 0;
float integral = 0;

//reach and maintain the given height
float quad_control_regulate_height(float targetHeight)
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
	debug_beep();
	
	throttle += (P*error)+integral*I;
	
	if(throttle>1600)
	throttle=1600;
	
	quad_output_set_throttle(throttle);
	
	//we might want to know how close we are to reaching the desired height, so return the difference just in case
	return error;
}

void quad_control_arm()
{
	quad_output_set_throttle(1000);
	quad_output_set_rudder(2000);
	_delay_ms(1000);
}

void quad_control_test_regulate_height()
{
	if(rx_get_aux_position()==1)
	{
		//regular pass through, except for Aux (keep at self-level mode)
			
		debug_println("Not regulating height!");
		quad_output_passthrough(true,true,true,true,true,false);
	}
	else if (rx_get_aux_position()==2)
	{
		//regular pass through, set target height to current height
		target_height = altimeter_get_metres();
		debug_println("Not regulating height!");
		quad_output_passthrough(true,true,true,true,true,false);
	}
	else if (rx_get_aux_position()==3)
	{
		//regulate height
		debug_println("regulating height!");
		quad_control_regulate_height(target_height);
		quad_output_passthrough(false,true,true,true,true,false);
	}
		
		quad_output_set_aux(1000);	//keep flight controller in self level mode
}