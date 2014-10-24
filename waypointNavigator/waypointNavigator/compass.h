
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


#ifndef COMPASS_H_
	#define COMPASS_H_

	extern "C" {
		#include "i2cmaster.h"
	}

	typedef enum
	{
		HMC5883_REGISTER_MAG_CRA_REG_M             = 0x00,
		HMC5883_REGISTER_MAG_CRB_REG_M             = 0x01,
		HMC5883_REGISTER_MAG_MR_REG_M              = 0x02,
		HMC5883_REGISTER_MAG_OUT_X_H_M             = 0x03,
		HMC5883_REGISTER_MAG_OUT_X_L_M             = 0x04,
		HMC5883_REGISTER_MAG_OUT_Z_H_M             = 0x05,
		HMC5883_REGISTER_MAG_OUT_Z_L_M             = 0x06,
		HMC5883_REGISTER_MAG_OUT_Y_H_M             = 0x07,
		HMC5883_REGISTER_MAG_OUT_Y_L_M             = 0x08,
		HMC5883_REGISTER_MAG_SR_REG_Mg             = 0x09,
		HMC5883_REGISTER_MAG_IRA_REG_M             = 0x0A,
		HMC5883_REGISTER_MAG_IRB_REG_M             = 0x0B,
		HMC5883_REGISTER_MAG_IRC_REG_M             = 0x0C,
		HMC5883_REGISTER_MAG_TEMP_OUT_H_M          = 0x31,
		HMC5883_REGISTER_MAG_TEMP_OUT_L_M          = 0x32
	} hmc5883MagRegisters_t;

	typedef enum
	{
		HMC5883_MAGGAIN_1_3 = 0x20,	//+/- 1.3
		HMC5883_MAGGAIN_1_9 = 0x40,	//+/- 1.9
		HMC5883_MAGGAIN_2_5 = 0x60,	//+/- 2.5
		HMC5883_MAGGAIN_4_0 = 0x80,	//+/- 4.0
		HMC5883_MAGGAIN_4_7 = 0xA0,	//+/- 4.7
		HMC5883_MAGGAIN_5_6 = 0xC0,	//+/- 5.6
		HMC5883_MAGGAIN_8_1 = 0xE0,	//+/- 8.1
		} hmc5883MagGain;

	#define HMC5883L_WRITE 0x3C // write address
	#define HMC5883L_READ 0x3D // read address
	#define DECLINATION 8 * PI / 180

	#define GAUSS_TO_MICROTESLA	(100)	//Gauss to micro-Tesla multiplier




	void init_HMC5883L(void);
	float compass_get_heading(void);
	void setMagGain(hmc5883MagGain);
	int16_t getHeadingTiltCompensated(void);

#endif /* COMPASS_H_ */
