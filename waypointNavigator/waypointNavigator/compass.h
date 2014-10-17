
/*
 * compass.h
 *
 * Created: 8/10/2014 1:38:13 PM
 *  Author: Chris Barr
 */ 
#include "main.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include "debug.h"


extern "C" {
	#include "i2cmaster.h"
}

#define HMC5883L_WRITE 0x3C // write address
#define HMC5883L_READ 0x3D // read address
#define DECLINATION 8 * PI / 180


void init_HMC5883L(void);
float getHeading(void);
int16_t getHeadingTiltCompensated(void);

int16_t iHundredAtan2Deg(int16_t val1, int16_t val2);
int16_t iHundredAtanDeg(int16_t val1, int16_t val2);
int16_t iTrig(int16_t val1, int16_t val2);
int16_t iDivide(int16_t iy, int16_t ix);