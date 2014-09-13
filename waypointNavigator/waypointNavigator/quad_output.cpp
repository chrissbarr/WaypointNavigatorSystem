/*
 * quad_output.cpp
 *
 * Created: 13/09/2014 7:05:31 PM
 *  Author: Chris
 */ 

#include "quad_output.h"

void quad_output_init()
{
	DDRH = 0xFF;  //set OC4x to output
	DDRE = 0xFF; //output

	//Initialize Timer/Counter 1
	//System Clock as source
	//Mode 8: Phase and Freq. Correct PWM top=ICR1
	//OC1A output: Non-inverted
	//OC1B output: currently disabled
	//OC1C output: currently disabled
	ICR4 = (F_CPU / (SERVO_HZ*8)) - 1;
	TCCR4A=(1<<COM4A1)|(1<<COM4B1)|(1<<COM4C1);//COM1A1 Clear OCnA when match counting up,Set on
	TCCR4B=(1<<WGM43)|(1<<CS41);// Phase and Freq correct ICR1=Top
	
	ICR3 = (F_CPU / (SERVO_HZ*8)) - 1;
	TCCR3A=(1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1);//COM1A1 Clear OCnA when match counting up,Set on
	TCCR3B=(1<<WGM33)|(1<<CS31);// Phase and Freq correct ICR1=Top
		
	THROTTLE_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
	AILERON_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
	ELEVATOR_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
	RUDDER_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
	GEAR_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
	AUX_OUT = DEFAULT_PWM_OUTPUT; //start at low value so vehicle is not armed
}


void quad_output_passthrough(bool throttle, bool aileron, bool elevator, bool rudder, bool gear, bool aux)
{
	if(throttle)
	{
		quad_output_set_throttle(rx_get_throttle());
	}
	if(aileron)
	{
		quad_output_set_aileron(rx_get_aileron());
	}
	if(elevator)
	{
		quad_output_set_elevator(rx_get_elevator());
	}
	if(rudder)
	{
		quad_output_set_rudder(rx_get_rudder());
	}
	if(gear)
	{
		quad_output_set_gear(rx_get_gear());
	}
	if(aux)
	{
		quad_output_set_aux(rx_get_aux());
	}
}

//setters for outputs
void quad_output_set_throttle(int throttle)
{
	THROTTLE_OUT = throttle;
}

void quad_output_set_aileron(int aileron)
{
	AILERON_OUT = aileron;
}

void quad_output_set_elevator(int elevator)
{
	ELEVATOR_OUT = elevator;
}

void quad_output_set_rudder(int rudder)
{
	RUDDER_OUT = rudder;
}

void quad_output_set_gear(int gear)
{
	GEAR_OUT = gear;
}

void quad_output_set_aux(int aux)
{
	AUX_OUT = aux;
}
