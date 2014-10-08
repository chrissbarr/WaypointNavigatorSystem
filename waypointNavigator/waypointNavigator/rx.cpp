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

volatile uint8_t portHistory = 0x00;     // default is low

//Pin Change Interrupt for reading Channel Values
ISR(PCINT0_vect)
{
	uint8_t changedBits;
	
	//determine which pin on RX_PORT changed:
	changedBits = RX_PORT ^ portHistory;
	portHistory = RX_PORT;
	
	
	//Now, operate for that pin:
	
	if(changedBits & (1 << THROTTLE_IN_PIN))	//if the pin assigned to throttle has changed (high->low or low->high)
	{
		if(RX_PORT & (1<<THROTTLE_IN_PIN))	//if the pin is high
		{
			ulThrottleStart = micros();	//note the current time
		}
		else	//if the pin has gone low
		{
			unThrottleInShared = (uint16_t)(micros()-ulThrottleStart);		//we are interested in how long it was high for
			rxUpdateFlagsShared |= THROTTLE_FLAG;		//and we need to let our code know that there is new data for the throttle
		}
	}
	
	if(changedBits & (1 << AILERON_IN_PIN))
	{
		if(RX_PORT & (1<<AILERON_IN_PIN))
		{
			ulAileronStart = micros();
		}
		else
		{
			unAileronInShared = (uint16_t)(micros()-ulAileronStart);
			rxUpdateFlagsShared |= AILERON_FLAG;
		}
	}
	
	if(changedBits & (1 << ELEVATOR_IN_PIN))
	{
		if(RX_PORT & (1<<ELEVATOR_IN_PIN))
		{
			ulElevatorStart = micros();
		}
		else
		{
			unElevatorInShared = (uint16_t)(micros()-ulElevatorStart);
			rxUpdateFlagsShared |= ELEVATOR_FLAG;
		}
	}
	
	if(changedBits & (1 << RUDDER_IN_PIN))
	{
		if(RX_PORT & (1<<RUDDER_IN_PIN))
		{
			ulRudderStart = micros();
		}
		else
		{
			unRudderInShared = (uint16_t)(micros()-ulRudderStart);
			rxUpdateFlagsShared |= RUDDER_FLAG;
		}
	}
	
	if(changedBits & (1 << GEAR_IN_PIN))
	{
		if(RX_PORT & (1<<GEAR_IN_PIN))
		{
			ulGearStart = micros();
		}
		else
		{
			unGearInShared = (uint16_t)(micros()-ulGearStart);
			rxUpdateFlagsShared |= GEAR_FLAG;
		}
	}
	
	if(changedBits & (1 << AUX_IN_PIN))
	{
		if(RX_PORT & (1<<AUX_IN_PIN))
		{
			ulAuxStart = micros();
		}
		else
		{
			unAuxInShared = (uint16_t)(micros()-ulAuxStart);
			rxUpdateFlagsShared |= AUX_FLAG;
		}
	}
}

void rx_init()
{
	DDRB = 0x00;
	sbi(DDRB,7);
	//Enable pin change interrupts
	PCMSK0 |= (1<<PCINT0);	//enable PCINT0
	PCMSK0 |= (1<<PCINT1);	//enable PCINT1
	PCMSK0 |= (1<<PCINT2);	//enable PCINT2
	PCMSK0 |= (1<<PCINT3);	//enable PCINT3
	PCMSK0 |= (1<<PCINT4);	//enable PCINT4
	PCMSK0 |= (1<<PCINT5);	//enable PCINT5
	PCICR |= (1<<PCIE0);	//enable pin change interrupts 0:7
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
		
		}
		
		if(rxUpdateFlags)
		{
			rxInputThrottle = unThrottleIn;
			rxInputAileron = unAileronIn;
			rxInputElevator = unElevatorIn;
			rxInputRudder = unRudderIn;
			rxInputGear = unGearIn;
			rxInputAux = unAuxIn;
			
			if(RX_VERBOSE_OUTPUT)
			{
				debug_printf(rx_get_throttle());
				debug_print("\t");
				debug_printf(rx_get_aileron());
				debug_print("\t");
				debug_printf(rx_get_elevator());
				debug_print("\t");
				debug_printf(rx_get_rudder());
				debug_print("\t");
				debug_printf(rx_get_gear());
				debug_print("\t");
				debug_printf(rx_get_aux());
				debug_print("\n\r");
			}
			
			rxUpdateFlags = 0;
		}
		
	
}

int rx_get_throttle()
{
	return rxInputThrottle;
}

float rx_get_aileron()
{
	return rxInputAileron;
}

float rx_get_elevator()
{
	return rxInputElevator;
}

float rx_get_rudder()
{
	return rxInputRudder;
}

float rx_get_gear()
{
	return rxInputGear;
}

float rx_get_aux()
{
	return rxInputAux;
}

//return a simplified result from the Aux switch,
//either position 1 (default), 2 or 3
int rx_get_aux_position()
{
	if(rx_get_aux()<1350)
		return 1;
	if(rx_get_aux()>=1350 && rx_get_aux()<=1650)
		return 2;
	if(rx_get_aux()>1650)
		return 3;
}