/*
 * debug.cpp
 *
 * Created: 6/09/2014 5:01:56 PM
 *  Author: Chris
 */ 

#include "debug.h"

void debug_print(const char* debugString)
{
	if(DEBUG == true)	//only print debug messages if debugging is enabled.
		USART_putstring(USART_PC,debugString);		//Just in case USART calls add overhead or slow system down too much later on, easy to disable all debug messages.
}

void debug_print(double debugNum)
{
	if(DEBUG == true)	//only print debug messages if debugging is enabled.
	{
		char string[10]; 
		sprintf(string,"%f",debugNum);
		USART_putstring(USART_PC,string);		
	}
}