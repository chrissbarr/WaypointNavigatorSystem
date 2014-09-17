/*
 * usart.cpp
 *
 * Created: 5/09/2014 6:05:54 PM
 *  Author: Chris
 */ 
#include "usart.h"


void USART_init(int usartNum, int baudRate)
{
	int baudPrescaler = (((F_CPU / (baudRate * 16UL))) - 1);
	
	if(usartNum == 0)
	{
		UBRR0H = (uint8_t)(baudPrescaler>>8);
		UBRR0L = (uint8_t)(baudPrescaler);
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);
		UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
	}
	else if(usartNum == 1)
	{
		UBRR1H = (uint8_t)(baudPrescaler>>8);
		UBRR1L = (uint8_t)(baudPrescaler);
		UCSR1B = (1<<RXEN1)|(1<<TXEN1);
		UCSR1C = ((1<<UCSZ10)|(1<<UCSZ11));
	}
	else if(usartNum == 2)
	{
		UBRR2H = (uint8_t)(baudPrescaler>>8);
		UBRR2L = (uint8_t)(baudPrescaler);
		UCSR2B = (1<<RXEN2)|(1<<TXEN2);
		UCSR2C = ((1<<UCSZ20)|(1<<UCSZ21));
	}
	else if(usartNum == 3)
	{
		UBRR3H = (uint8_t)(baudPrescaler>>8);
		UBRR3L = (uint8_t)(baudPrescaler);
		UCSR3B = (1<<RXEN3)|(1<<TXEN3);
		UCSR3C = ((1<<UCSZ30)|(1<<UCSZ31));
	}
	
}

void USART_send(int usartNum, unsigned char data){
	
	if(usartNum == 0)
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = data;
	}
	else if(usartNum == 1)
	{
		while(!(UCSR1A & (1<<UDRE1)));
		UDR1 = data;
	}
	else if(usartNum == 2)
	{
		while(!(UCSR2A & (1<<UDRE2)));
		UDR2 = data;
	}
	else if(usartNum == 3)
	{
		while(!(UCSR3A & (1<<UDRE3)));
		UDR3 = data;
	}
}

unsigned char USART_receive(int usartNum){
	if(usartNum == 0)
	{
		while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
	}
	else if(usartNum == 1)
	{
		while(!(UCSR1A & (1<<RXC1)));
		return UDR1;
	}
	else if(usartNum == 2)
	{
		while(!(UCSR2A & (1<<RXC2)));
		return UDR2;
	}
	else
	{
		while(!(UCSR3A & (1<<RXC3)));
		return UDR3;
	}
	
}

/*//Receive string through UART
unsigned char * USART_receive_string(int usartNum)
{
	unsigned char string[20], x, i = 0;

	//receive the characters until ENTER is pressed (ASCII for ENTER = 13)
	while((x = USART_receive(usartNum)) != 13)
	{
		//and store the received characters into the array string[] one-by-one
		string[i++] = x;
	}

	//insert NULL to terminate the string
	string[i] = '\0';

	//return the received string
	return(string);
}*/

void USART_putstring(int usartNum, const char* StringPtr){
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_send(usartNum, *StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}
