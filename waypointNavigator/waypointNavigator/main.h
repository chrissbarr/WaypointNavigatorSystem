/*
 * main.h
 *
 * Created: 5/09/2014 7:17:26 PM
 *  Author: Chris
 */ 


#define F_CPU  16000000L		//16MHz clock rate, needed for timing functions

//enable or disable debug features. Debug features are useful but might slow AVR down, best to turn them off when not needed.
#define DEBUG_ENABLE true



//defining ports different components will use.
#define GPS_PORT PIND //USART1 is on Port D, needed to communicate with GPS module
#define RX_PORT PINB	//port RX is connected to, PORTB has Pin Change interrupts
#define BARO_PORT PINC  //barometer communicates via I2C.
#define COMP_PORT PINE  //computer connects via USART0 for debugging.
#define LCD_PORT PORTG
#define KEY_PORT PINF
#define FC_PORT PORTH //requires PWM outputs


