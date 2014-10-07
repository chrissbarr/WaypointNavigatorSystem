/*
 * altimeter.cpp
 *
 * Created: 17/09/2014 5:15:48 PM
 *  Author: Chris
 * Borrowed from http://www.avrfreaks.net/forum/solved-fleury-i2c-problems-mpl3115a2?skey=MPL3115A2
 * Also heavily based on the Sparkfun MPL3115A2 Arduino example code.
 */ 

#include "altimeter.h"


#define MPL3115a2 0xC0 

uint8_t altStatus = 0x00;
float altitude = 0.;
float pressure = 0.;
float temperature = 0.;
float start_height = 0;

bool altimeter_init()
{
	i2c_init();
	bool success = true;
	
	if(IIC_Read(WHO_AM_I) == 196)
	{
		debug_println("Altimeter connected correctly!");
	}
	else
	{
		debug_println("Altimeter not connected!");
		success = false;
	}
	
	if(success)
	{
		setModeAltimeter();
		setOversampleRate(7);
		enableEventFlags();
	
		
		start_height = altimeter_start_height();
		
		if(start_height == -999)
			success = false;
			
		if(success)
			debug_println("Altimeter initialised correctly!");
		else
			debug_println("Altimeter failed to initialise!");
	}
	
	return success;	
}

float altimeter_start_height()
{
	float start_height = 0;
	int start_height_samples = 0;
	
	debug_println("Determining Altimeter Start height, sampling for 1s...");
	//sample the current height for 1 second
	while(millis()<3000)
	{
		start_height += altimeter_get_metres();
		start_height_samples += 1;
	}
	
	//average the height
	start_height = start_height / start_height_samples;
	debug_print("Starting height initialised to: ");
	debug_printf(start_height);
	debug_println(" m");
	
	return start_height;
}

float altimeter_get_metres(void)
{
	float altitude = -999;
	
	altimeter_toggle_oneShot();
	
	int counter = 0;
	
	while((IIC_Read(STATUS) & (1<<1)) == 0)
	{
		if(++counter > 600) return (-999);
		_delay_ms(1);
	}
	//_delay_ms(100);
	
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(OUT_P_MSB);
	i2c_rep_start(MPL3115a2+I2C_READ);
	
	int8_t msbA,csbA,lsbA = 0x00; 
	
	msbA = i2c_readAck();
	csbA = i2c_readAck();
	lsbA = i2c_readNak();	
	i2c_stop();
	
	altimeter_toggle_oneShot();
	
	float tempcsb = (lsbA>>4)/16.0;
	
	altitude = (float)( (msbA << 8) | csbA) + tempcsb;
	
	return altitude;
}

void altimeter_toggle_oneShot(void) 
{ 
	//read CTRL_REG1
	uint8_t tempSetting = IIC_Read(CTRL_REG1);
	
	//Clear OST bit
	tempSetting &= ~(1<<1);
	IIC_Write(CTRL_REG1,tempSetting);
	
	//read CTRL_REG1
	tempSetting = IIC_Read(CTRL_REG1);
	
	//set OST bit
	tempSetting |= (1<<1);
	IIC_Write(CTRL_REG1,tempSetting); 
	
}

void setModeAltimeter()
{
	//read CTRL_REG1
	uint8_t tempSetting = IIC_Read(CTRL_REG1);
	
	tempSetting |= (1<<7);	//set ALT bit
	
	IIC_Write(CTRL_REG1,tempSetting);
}

void setOversampleRate(uint8_t sampleRate)
{
	if(sampleRate > 7) sampleRate = 7;	//OS cannot be large than 0b.0111
	sampleRate <<= 3;
	
	//read CTRL_REG1
	uint8_t tempSetting = IIC_Read(CTRL_REG1);
	
	tempSetting &= 0b11000111; //Clear out old OS bits
	tempSetting |= sampleRate; //Mask in new OS bits
	
	IIC_Write(CTRL_REG1,tempSetting);
	
}

//getter for start height
float altimeter_get_start_height()
{
	return start_height;
}

//Enables the pressure and temp measurement event flags so that we can
//test against them. This is recommended in datasheet during setup.
void enableEventFlags()
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(PT_DATA_CFG);
	i2c_write(0x07);
}

byte IIC_Read(byte regAddr)
{
	// This function reads one byte over IIC
	
	i2c_rep_start(MPL3115a2+I2C_WRITE);
	i2c_write(regAddr);
	
	i2c_rep_start(MPL3115a2+I2C_READ);
	return i2c_readNak();
}

void IIC_Write(byte regAddr, byte value)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(regAddr);
	i2c_write(value);
	i2c_stop();
}