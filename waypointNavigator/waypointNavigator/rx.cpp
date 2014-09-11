/*
 * rx.cpp
 *
 * Created: 6/09/2014 4:57:43 PM
 *  Author: Chris
 */ 

#include "rx.h"

//shared variables used in interrupts
volatile uint16_t unThrottleInShared;
volatile uint16_t unAileronInShared;
volatile uint16_t unElevatorInShared;
volatile uint16_t unRudderInShared;
volatile uint16_t unGearInShared;
volatile uint16_t unAuxInShared;

//tracks if a channel has changed values
volatile uint8_t rxUpdateFlagsShared;

//variables used outside of interrupts (non-volatile)
uint32_t ulThrottleStart;
uint32_t ulAileronStart;
uint32_t ulElevatorStart;
uint32_t ulRudderStart;
uint32_t ulGearStart;
uint32_t ulAuxStart;

//create variables to be accessed later by other functions
uint16_t rxInputThrottle;
uint16_t rxInputAileron;
uint16_t rxInputElevator;
uint16_t rxInputRudder;
uint16_t rxInputGear;
uint16_t rxInputAux;

//Pin Change Interrupt for reading Throttle Value//
ISR(PCINT0_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(THROTTLE_IN_PIN))
	{
		ulThrottleStart = micros();
	}
	else
	{
		unThrottleInShared = (uint16_t)(micros()-ulThrottleStart);
		rxUpdateFlagsShared |= THROTTLE_FLAG;
	}
	
}

//Pin Change Interrupt for reading Aileron Value//
ISR(PCINT1_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(AILERON_IN_PIN))
	{
		ulAileronStart = micros();
	}
	else
	{
		unAileronInShared = (uint16_t)(micros()-ulAileronStart);
		rxUpdateFlagsShared |= AILERON_FLAG;
	}
	
}

//Pin Change Interrupt for reading Elevator Value//
ISR(PCINT2_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(ELEVATOR_IN_PIN))
	{
		ulElevatorStart = micros();
	}
	else
	{
		unElevatorInShared = (uint16_t)(micros()-ulElevatorStart);
		rxUpdateFlagsShared |= ELEVATOR_FLAG;
	}
	
}

//Pin Change Interrupt for reading Rudder Value//
/*ISR(PCINT2_vect)
{
	//if pin input is attached to is high
	if(RX_PORT & _BV(RUDDER_IN_PIN))
	{
		ulRudderStart = micros();
	}
	else
	{
		unRudderInShared = (uint16_t)(micros()-ulRudderStart);
		rxUpdateFlagsShared |= RUDDER_FLAG;
	}
	
}*/



void rx_init()
{
	
	
	sei(); //enable interrupts
	
}


//check each radio channel. If it's changed since last time, update the corresponding variable
void rx_update()
{
		
	  static uint16_t unThrottleIn;
	  static uint16_t unAileronIn;
	  static uint16_t unElevatorIn;
	  static uint16_t unRudderIn;
	  static uint16_t unGearIn;
	  static uint16_t unAuxIn;
	  
	  // local copy of update flags
	  static uint8_t rxUpdateFlags;
	  
	if(rxUpdateFlagsShared)
	{
		cli();	//disable all interrupts. Can't have something change now!
		
		rxUpdateFlags = rxUpdateFlagsShared;	//retrieve data on which channel changed from the volatile variable.
		
		if(rxUpdateFlags & THROTTLE_FLAG)
		{
			unThrottleIn = unThrottleInShared;
		}
		
		if(rxUpdateFlags & AILERON_FLAG)
		{
			unAileronIn = unAileronInShared;
		}
		
		if(rxUpdateFlags & ELEVATOR_FLAG)
		{
			unElevatorIn = unElevatorInShared;
		}
		
		if(rxUpdateFlags & RUDDER_FLAG)
		{
			unRudderIn = unRudderInShared;
		}
		
		if(rxUpdateFlags & GEAR_FLAG)
		{
			unGearIn = unGearInShared;
		}
		
		if(rxUpdateFlags & AUX_FLAG)
		{
			unAuxIn = unAuxInShared;
		}
		
		rxUpdateFlagsShared = 0;
		
		sei();	//all needed data is now in local variables. We can safely re-enable interrupts!
		
		rxUpdateFlags = 0;
		
		rxInputThrottle = unThrottleIn;
		rxInputAileron = unAileronIn;
		rxInputElevator = unElevatorIn;
		rxInputRudder = unRudderIn;
		rxInputGear = unGearIn;
		rxInputAux = unAuxIn;
	}
}
