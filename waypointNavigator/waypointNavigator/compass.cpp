/*
 * COMPLASSZ.cpp
 *
 * Created: 3/10/2014 10:26:05 AM
 *  Author: axzqy008
 */ 

#include "compass.h"

//compass raw data
uint8_t mag_raw_xhi = 0;
uint8_t mag_raw_xlo = 0;

uint8_t mag_raw_zhi = 0;
uint8_t mag_raw_zlo = 0;

uint8_t mag_raw_yhi = 0;
uint8_t mag_raw_ylo = 0;

//compass data
int16_t mag_data_x, mag_data_y, mag_data_z;

float mag_proc_x, mag_proc_y, mag_proc_z;

hmc5883MagGain _magGain;


static float _hmc5883_Gauss_LSB_XY = 1100.0F;
static float _hmc5883_Gauss_LSB_Z = 980.0F;

void init_HMC5883L(void){

	i2c_start(HMC5883L_WRITE);
	i2c_write(HMC5883_REGISTER_MAG_MR_REG_M);
	i2c_write(0x00);
	
	setMagGain(HMC5883_MAGGAIN_1_3);
}

float compass_get_heading(void){

	i2c_start_wait(HMC5883L_WRITE);
	i2c_write(0x03); //set pointer to X-axis MSB
	i2c_stop();

	i2c_rep_start(HMC5883L_READ);

	mag_raw_xhi = i2c_readAck();
	mag_raw_xlo = i2c_readAck();
	
	mag_raw_zhi = i2c_readAck();
	mag_raw_zlo = i2c_readAck();
	
	mag_raw_yhi = i2c_readAck();
	mag_raw_ylo = i2c_readNak();
	
	i2c_stop();
	
	//shift values to create properly formed integer (low byte first)
	mag_data_x = (int16_t) (mag_raw_xlo | (int16_t)mag_raw_xhi << 8);
	mag_data_y = (int16_t) (mag_raw_ylo | (int16_t)mag_raw_yhi << 8);
	mag_data_z = (int16_t) (mag_raw_zlo | (int16_t)mag_raw_zhi << 8);
	
	mag_proc_x = mag_data_x / _hmc5883_Gauss_LSB_XY * GAUSS_TO_MICROTESLA;
	mag_proc_y = mag_data_y / _hmc5883_Gauss_LSB_XY * GAUSS_TO_MICROTESLA;
	mag_proc_z = mag_data_z / _hmc5883_Gauss_LSB_Z * GAUSS_TO_MICROTESLA;
	
	//debug_print("X: ");debug_printf(mag_proc_x);debug_print(" ");
	//debug_print("Y: ");debug_printf(mag_proc_y);debug_print(" ");
	//debug_print("Z: ");debug_printf(mag_proc_z);debug_print(" ");debug_println("uT");
	
	//calculate heading
	float heading = atan2(mag_proc_y,mag_proc_x);
	
	heading += DECLINATION;
	
	if(heading<0)
		heading+=2*PI;
	
	if(heading > 2*PI)
		heading-=2*PI;
		
	float heading_degrees = heading * 180 / PI;
	
	//debug_print("Heading (degrees): "); debug_printf(heading_degrees);debug_println("");
	
	return heading_degrees;
}

void setMagGain(hmc5883MagGain gain)
{
	i2c_start(HMC5883L_WRITE);
	i2c_write(HMC5883_REGISTER_MAG_CRB_REG_M); // set pointer
	i2c_write((byte)gain); // write gain
	i2c_stop();
	
	_magGain = gain;
	
	switch(gain)
	{
		case HMC5883_MAGGAIN_1_3:
			_hmc5883_Gauss_LSB_XY = 1100;
			_hmc5883_Gauss_LSB_Z  = 980;
			break;
		case HMC5883_MAGGAIN_1_9:
			_hmc5883_Gauss_LSB_XY = 855;
			_hmc5883_Gauss_LSB_Z  = 760;
			break;
		case HMC5883_MAGGAIN_2_5:
			_hmc5883_Gauss_LSB_XY = 670;
			_hmc5883_Gauss_LSB_Z  = 600;
			break;
		case HMC5883_MAGGAIN_4_0:
			_hmc5883_Gauss_LSB_XY = 450;
			_hmc5883_Gauss_LSB_Z  = 400;
			break;
		case HMC5883_MAGGAIN_4_7:
			_hmc5883_Gauss_LSB_XY = 400;
			_hmc5883_Gauss_LSB_Z  = 255;
			break;
		case HMC5883_MAGGAIN_5_6:
			_hmc5883_Gauss_LSB_XY = 330;
			_hmc5883_Gauss_LSB_Z  = 295;
			break;
		case HMC5883_MAGGAIN_8_1:
			_hmc5883_Gauss_LSB_XY = 230;
			_hmc5883_Gauss_LSB_Z  = 205;
			break;
	}

}

int16_t getHeadingTiltCompensated(void)
{
	return -999;
}
