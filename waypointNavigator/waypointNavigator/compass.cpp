/*
 * COMPLASSZ.cpp
 *
 * Created: 3/10/2014 10:26:05 AM
 *  Author: axzqy008
 */ 

#include "compass.h"

int16_t raw_x = 0;
int16_t raw_y = 0;
int16_t raw_z = 0;

void init_HMC5883L(void){

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x00); // set pointer to CRA
	i2c_write(0x70); // write 0x70 to CRA
	i2c_stop();

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x01); // set pointer to CRB
	i2c_write(0xA0);
	i2c_stop();

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x02); // set pointer to measurement mode
	i2c_write(0x00); // continuous measurement
	i2c_stop();
}

float getHeading(void){

	i2c_start_wait(HMC5883L_WRITE);
	i2c_write(0x03); //set pointer to X-axis MSB
	i2c_stop();

	i2c_rep_start(HMC5883L_READ);

	raw_x = ((uint8_t)i2c_readAck())<<8;
	raw_x |= i2c_readAck();

	raw_z = ((uint8_t)i2c_readAck())<<8;
	raw_z |= i2c_readAck();

	raw_y = ((uint8_t)i2c_readAck())<<8;
	raw_y |= i2c_readNak();

	i2c_stop();
	
	float heading = atan2(raw_y,raw_x)/2;
	float heading_dec = heading+DECLINATION;
	
	/*
	if(heading < 0)
		heading += 2*PI;
	
	if(heading>2*PI)
		heading-=2*PI;
		
	if(heading_dec < 0)
	heading_dec += 2*PI;
	
	if(heading_dec>2*PI)
	heading_dec-=2*PI;
	*/
		
	float heading_degrees = heading * 180 / PI;
	float heading_dec_degrees = heading_dec * 180 / PI;
	
	debug_print("Raw x: ");
	debug_printf(raw_x);
	debug_print("\t Heading (radians): ");
	debug_printf(heading);
	debug_print("\t Heading (degrees): ");
	debug_printf(heading_degrees);
	debug_print("\t Heading (w/Declination): ");
	debug_printf(heading_dec_degrees);
	debug_println("");
	
	
	return (atan2((double)raw_y,(double)raw_x)* 180) / 3.14159265 + 180 + DECLINATION;
}
