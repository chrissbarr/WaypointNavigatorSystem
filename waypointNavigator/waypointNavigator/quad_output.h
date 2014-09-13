/*
 * quad_output.h
 *
 * Created: 13/09/2014 7:05:40 PM
 *  Author: Chris
 */ 

#include "main.h"
#include <avr/io.h>
#include "rx.h"

#define SERVO_HZ 50
#define DEFAULT_PWM_OUTPUT 950

#define THROTTLE_OUT OCR4A	//PH3
#define AILERON_OUT OCR4B	//PH4
#define ELEVATOR_OUT OCR4C	//PH5
#define RUDDER_OUT OCR3A	//PE3
#define GEAR_OUT OCR3B		//PE4
#define AUX_OUT OCR3C		//PE5


void quad_output_init();

void quad_output_passthrough(bool throttle, bool aileron, bool elevator, bool rudder, bool gear, bool aux);

//setters for outputs
void quad_output_set_throttle(int throttle);
void quad_output_set_aileron(int aileron);
void quad_output_set_elevator(int elevator);
void quad_output_set_rudder(int rudder);
void quad_output_set_gear(int gear);
void quad_output_set_aux(int aux);
