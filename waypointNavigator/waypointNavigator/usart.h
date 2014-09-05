/*
 * usart.h
 *
 * Created: 5/09/2014 6:06:05 PM
 *  Author: Chris
 */ 

#include <avr/io.h>#include "main.h"
#define BAUD 9600		//baud rate for serial communications, PC and GPS can use same baud rate (9600).
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths

//define which USART channel is what
#define USART_PC 0
#define USART_GPS 1

//USART FUNCTIONS
void USART_init(int usartNum, int baudRate);
void USART_send(int usartNum, unsigned char data);
unsigned char USART_receive(int usartNum);
void USART_putstring(int usartNum, const char* StringPtr);
