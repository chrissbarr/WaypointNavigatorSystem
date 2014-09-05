/*
 * usart.cpp
 *
 * Created: 5/09/2014 6:05:54 PM
 *  Author: Chris
 */ 
#include "usart.h"

//#define F_CPU  16000000L		//16MHz clock rate, needed for timing functions
#define BAUD 9600		//baud rate for serial communications, needed for serial to PC
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths


void USART_init()
{
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

void USART_putstring(const char* StringPtr){
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_send(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}