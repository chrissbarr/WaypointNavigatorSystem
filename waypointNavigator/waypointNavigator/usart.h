/*
 * usart.h
 *
 * Created: 5/09/2014 6:06:05 PM
 *  Author: Chris
 */ 

#include <avr/io.h>#include "main.h"

//USART FUNCTIONS
void USART_init();
void USART_send( unsigned char data);
unsigned char USART_receive(void);
void USART_putstring(const char* StringPtr);
