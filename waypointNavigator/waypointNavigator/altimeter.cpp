/*
 * altimeter.cpp
 *
 * Created: 17/09/2014 5:15:48 PM
 *  Author: Chris
 * Borrowed from http://www.avrfreaks.net/forum/solved-fleury-i2c-problems-mpl3115a2?skey=MPL3115A2
 */ 

#include "altimeter.h"


#define MPL3115a2 0xC0 

uint8_t altStatus = 0x00;
float altitude = 0.;
float pressure = 0.;
float temperature = 0.;

void altimeter_init()
{
	mpl_init();
	debug_println("Altimeter initialized!");
}

void mpl_init (void) 
{ 
   i2c_init(); 
   altimeter_set_mode();
   altimeter_set_eventFlags(); 
} 

void altimeter_set_mode (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(CTRL_REG1);
	i2c_write(0xB8);
	i2c_stop();
}

void altimeter_set_eventFlags (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(PT_DATA_CFG);
	i2c_write(0x07);      // Enable all 3 pressure and temp flags
	i2c_stop();
}

float mpl_getAlt (uint8_t altStatus) 
{    
   long temp = 0;
   
   altimeter_set_active();
    
   altimeter_get_status(); 
    
   int8_t msbA,csbA,lsbA,msbT,lsbT = 0x00; 
       
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(OUT_P_MSB); 
   i2c_rep_start(MPL3115a2+I2C_READ); 
   //_delay_ms(10);
   msbA = i2c_readAck(); 
   csbA = i2c_readAck(); 
   lsbA = i2c_readAck();
   msbT = i2c_readAck();
   lsbT = i2c_readNak(); 
   i2c_stop(); 
   
    if(msbA > 0x7F) 
	{
	    temp = ~((long)msbA << 16 | (long)csbA << 8 | (long)lsbA) + 1; // 2's complement the data
	    altitude = (float) (temp >> 8) + (float) ((lsbA >> 4)/16.0); // Whole number plus fraction altitude in meters for negative altitude
	    altitude *= -1.;
    }
    else 
	{
	    temp = ((msbA << 8) | csbA);
	    altitude = (float) (temp) + (float) ((lsbA >> 4)/16.0);  // Whole number plus fraction altitude in meters
    }
	
	long pressure_whole =  ((long)msbA << 16 | (long)csbA << 8 | (long)lsbA) ; // Construct whole number pressure
	pressure_whole >>= 6;
	
	lsbA &= 0x30;
	lsbA >>= 4;
	float pressure_frac = (float) lsbA/4.0;

	pressure = (float) (pressure_whole) + pressure_frac;

if(msbT > 0x7F) 
	{
		temp = ~(msbT << 8 | lsbT) + 1 ; // 2's complement
		temperature = (float) (temp >> 8) + (float)((lsbT >> 4)/16.0); // add whole and fractional degrees Centigrade
		temperature *= -1.;
	}
else 
	{
		temperature = (float) (msbT) + (float)((lsbT >> 4)/16.0); // add whole and fractional degrees Centigrade
	}
    
   if (temperature < 20) 
   { 
       PINB = 0x30;
   } 
   
   return altitude;
} 

void altimeter_set_active (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(CTRL_REG1);
	i2c_write(0xB9);
	i2c_stop();
}

uint8_t altimeter_get_status (void) 
{ 
   uint8_t altStatus = 0x00; 
	int i = 0;
   while (((altStatus & 0x08) == 0) || (i <= 10))
   { 
      i2c_start_wait(MPL3115a2+I2C_WRITE); 
      i2c_write(STATUS); 
      i2c_rep_start(MPL3115a2+I2C_READ); 
      altStatus = i2c_readNak(); 
      i2c_stop(); 
	  _delay_us(100);
	  i++;
   } 
   //DDRB |= 0x30; PORTB |= 0x30;; 
   return altStatus;
} 

/*void altimeter_toggle_oneShot (void) 
{ 
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(CTRL_REG1); 
   i2c_write(0xB8); 
    
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(CTRL_REG1); 
   i2c_write(0xBB); 
   i2c_stop(); 
}*/ 