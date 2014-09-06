/*
 * rx.cpp
 *
 * Created: 6/09/2014 4:57:43 PM
 *  Author: Chris
 */ 

#include "rx.h"

//shared variables used in interrupts
volatile uint16_t unThrottleInShared;
volatile uint8_t rxUpdateFlagsShared;

//variables used outside of interrupts (non-volatile)
uint32_t ulThrottleStart;

static uint8_t rxUpdateFlags;

void rx_init()
{
	//
}



/*
//Pin Change Interrupt for reading Throttle Value//
ISR(PCINT0_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(THROTTLE_IN_PIN))
	{
		//ulThrottleStart = micros();
	}
	else
	{
		//unThrottleInShared = (uint16_t)(micros()-ulThrottleStart);
		rxUpdateFlagsShared |= THROTTLE_FLAG;
	}
	
}
*/