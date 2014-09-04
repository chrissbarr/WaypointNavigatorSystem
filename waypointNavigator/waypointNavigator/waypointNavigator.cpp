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

#define F_CPU  16000000L		//16MHz clock rate, needed for timing functions
#define BAUD 9600		//baud rate for serial communications, needed for serial to PC
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths




//defining ports different components will use.
//I believe inputs use PINx for their name, 
//whereas outputs use PORTx. I have had this
//in mind with the following definitions.
//If I'm wrong, we can change it later easily here.

#define RX_PORT PINA	//port RX is connected to
#define GPS_PORT PINB	
#define BARO_PORT PINC	
#define COMP_PORT PIND	
#define LCD_PORT PORTE	
#define KEY_PORT PINF	
#define FC_PORT PORTG	


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <util/setbaud.h>
#include <stdio.h>

//our included files

#include "timing.h"		//mash up of timing stuff borrowed from Arduino source files


	///////////////INPUT FUNCTIONS/////////////////////
//-------------GPS INPUT FUNCTION PROTOTYPES---------------//
void init_gps();			//initializes GPS system, begins communication
bool gps_locked();			//checks if GPS is locked and data is available
void gps_update_values();	//gets data from GPS and stores relevant data in variables

//----------BAROMETER INPUT FUNCTION PROTOTYPES------------//
void init_baro();		//setup barometer and begin communication
float get_baro_reading();	//returns raw reading from barometer
float get_baro_height();	//calculates height from barometer value 

//-----------COMPASS INPUT FUNCTION PROTOTYPES-------------//
void init_compass();	//initializes compass module
float get_compass_heading();	//returns current heading, formatted as degrees relative to North (0-360)

//--------------RX INPUT FUNCTION PROTOTYPES---------------//
void init_rx_input();

//-------------RX Input Variables-----------------------//


#define THROTTLE_IN_PIN PB0		//Which RX channels map to which pins in port
#define AILERON_IN_PIN PB1
#define ELEVATOR_IN_PIN PB2
#define RUDDER_IN_PIN PB3
#define GEAR_IN_PIN PB4
#define AUX_IN_PIN PB5

//flags to represent changes in different channels
#define THROTTLE_FLAG 1		
#define AILERON_FLAG 2
#define ELEVATOR_FLAG 4
#define RUDDER_FLAG 8
#define GEAR_FLAG 16
#define AUX_FLAG 32

//shared variables used in interrupts
volatile uint16_t unThrottleInShared;
volatile uint8_t rxUpdateFlagsShared;

//variables used outside of interrupts (non-volatile)
uint32_t ulThrottleStart;
static uint8_t rxUpdateFlags;


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

//USART Functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);



//---------------WAYPOINT CORE PROTOTYPES---------------//
void init_waypoint_core();	//initialises arrays for long and lat, and sets waypoint index to zero
float waypoint_get_distance(float lat1, float long1, float lat2, float long2);	//returns the straight-line distance between two coordinates
float waypoint_get_angle(float lat1, float long1, float lat2, float long2);	//return an angle or heading from first pair to second pair of coordinates


int main(void)
{
	char String[]="Test string.";

	USART_init();
	
	//Initialise Reading Radio Receiver 
	init_rx_input();

    while(1)
    {
		 USART_putstring(String);    //Pass the string to the USART_putstring function and sends it over the serial
		 _delay_ms(5000);
    }
}

void init_rx_input()
{
	//Enable Pin Change Interrupts
		//todo
		
	initTimers();
} 


//USART FUNCTIONS
void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_send(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}
