/*
 * rx.h
 *
 * Created: 6/09/2014 4:58:29 PM
 *  Author: Chris
 */ 

#include "main.h"
#include <avr/io.h>
#include "timing.h"


//--------------RX INPUT FUNCTION PROTOTYPES---------------//
void rx_init();
void rx_update();

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

