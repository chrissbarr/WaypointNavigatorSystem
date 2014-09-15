/*
 * compass.cpp
 *
 * Created: 15/09/2014 7:43:10 PM
 */ 

#include "i2cmaster/i2cmaster.h"

#define HMC5883L_WRITE 0x3C
#define HMC5883L_READ 0x3D

bool compass_init()
{
	i2c_init();                             // initialize I2C library
	
	i2c_start_wait(HMC5883L_WRITE+I2C_WRITE);     // set device address and write mode
	
	//first line (send 0x3C 0x00 0x70), 0x3C is already sent by library when we define i2c_start_wait above
	i2c_write(0x00);                        // write address = register0
	i2c_write(0x70);                        // write value 0x70 to register
	
	//second line (send 0x3C 0x01 0xA0)
	i2c_write(0x01);                        // write address = register1
	i2c_write(0xA0);                        // write value 0xA0 to register
}

void compass_read()
{
	
	i2c_start_wait(HMC5883L_WRITE+I2C_WRITE);     // set device address and write mode
	i2c_write(0x02);                        // write address = register2
	i2c_write(0x01);                        // write value 0x01 to register
	
	//we've told the compass we want a reading. It takes 6ms to respond (from the datasheet), so let's wait 6ms to be safe.
	_delay_ms(6);
	
	//now, set where we are reading from (register 0x06)
	i2c_write(0x06);  
	
	//Now we need to read the value back. Let's change from writing to reading:
	i2c_start_wait(HMC5883L_READ+I2C_READ);     // set device address and read mode
	
	//and read the value. From the datasheet, the information comes in 6 bytes, representing X, Y and Z values. So, we need two variables for each:
	int x_highbyte, x_lowbyte, y_highbyte, y_lowbyte, z_highbyte, z_lowbyte;
	
	//this reads one byte
	x_highbyte = i2c_readAck();                    // read one byte from compass
	x_lowbyte = i2c_readAck();                    // read one byte from compass
	
	y_highbyte = i2c_readAck();                    // read one byte from compass
	y_lowbyte = i2c_readAck();                    // read one byte from compass
	
	z_highbyte = i2c_readAck();                    // read one byte from compass
	z_lowbyte = i2c_readNak();                    // read one byte from compass, stop reading (NAK)
	
}