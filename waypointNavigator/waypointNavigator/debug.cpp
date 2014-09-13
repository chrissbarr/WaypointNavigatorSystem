/*
 * debug.cpp
 *
 * Created: 6/09/2014 5:01:56 PM
 *  Author: Chris
 */ 

#include "debug.h"

void debug_print(const char* debugString)
{
	if(DEBUG_ENABLE == true)	//only print debug messages if debugging is enabled.
	{
		USART_putstring(USART_PC,debugString);		//Just in case USART calls add overhead or slow system down too much later on, easy to disable all debug messages.
		//USART_putstring(USART_PC,"\n\r");
	}
}

void debug_println(const char* debugString)
{
	if(DEBUG_ENABLE == true)	//only print debug messages if debugging is enabled.
	{
		USART_putstring(USART_PC,debugString);		//Just in case USART calls add overhead or slow system down too much later on, easy to disable all debug messages.
		USART_putstring(USART_PC,"\n\r");
	}
}

void debug_printi(int debugNum)
{
	if(DEBUG_ENABLE == true)	//only print debug messages if debugging is enabled.
	{
		char string[10]; 
		itoa(debugNum,string,10);
		debug_print(string);		
	}
}

void debug_printf(float debugNum)
{
	if(DEBUG_ENABLE == true)
	{
		char string[20];
		dtostrf(debugNum,10,5,string);
		debug_print(string);
	}
}
