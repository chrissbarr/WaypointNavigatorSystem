/*
 * main.h
 *
 * Created: 5/09/2014 7:17:26 PM
 *  Author: Chris
 */ 


#define F_CPU  16000000L		//16MHz clock rate, needed for timing functions


//defining ports different components will use.
#define RX_PORT PINA	//port RX is connected to
#define GPS_PORT PINB
#define BARO_PORT PINC
#define COMP_PORT PIND
#define LCD_PORT PORTE
#define KEY_PORT PINF
#define FC_PORT PORTG