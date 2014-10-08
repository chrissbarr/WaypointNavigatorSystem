/*
 * rx.h
 *
 * Created: 6/09/2014 4:58:29 PM
 *  Author: Chris
 */ 

#include "main.h"
#include <avr/io.h>
#include "timing.h"
#include "debug.h"

#define RX_VERBOSE_OUTPUT false

//--------------RX INPUT FUNCTION PROTOTYPES---------------//
void rx_init();
void rx_update();

int rx_get_throttle();
float rx_get_aileron();
float rx_get_elevator();
float rx_get_rudder();
float rx_get_gear();
float rx_get_aux();

int rx_get_aux_position();

//-------------RX Input Variables-----------------------//
//Which RX channels map to which pins in port
#define THROTTLE_IN_PIN PB0		//Digital Pin 53
#define AILERON_IN_PIN PB1		//Digital Pin 52
#define ELEVATOR_IN_PIN PB2		//Digital Pin 51
#define RUDDER_IN_PIN PB3		//Digital Pin 50
#define GEAR_IN_PIN PB4			//Digital Pin 10
#define AUX_IN_PIN PB5			//Digital Pin 11

//flags to represent changes in different channels
#define THROTTLE_FLAG 1
#define AILERON_FLAG 2
#define ELEVATOR_FLAG 4
#define RUDDER_FLAG 8
#define GEAR_FLAG 16
#define AUX_FLAG 32

