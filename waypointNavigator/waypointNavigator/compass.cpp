/*
 * COMPLASSZ.cpp
 *
 * Created: 3/10/2014 10:26:05 AM
 *  Author: axzqy008
 */ 

#include "compass.h"

int16_t mag_raw_x = 0;
int16_t mag_raw_y = 0;
int16_t mag_raw_z = 0;

int16_t accel_raw_x = 0;
int16_t accel_raw_y = 0;
int16_t accel_raw_z = 0;

int16_t fixed_x = 0;
int16_t fixed_y = 0;
int16_t fixed_z = 0;

int16_t k1x = 1;
int16_t k2x = 13.52;

int16_t k1y = 1;
int16_t k2y = -137.703532;

	/* roll pitch and yaw angles computed by iecompass */
	static int16_t iPhi, iThe, iPsi;
	/* magnetic field readings corrected for hard iron effects and PCB orientation */
	static int16_t iBfx, iBfy, iBfz;
	/* hard iron estimate */
	static int16_t iVx, iVy, iVz;

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

	mag_raw_x = ((uint8_t)i2c_readAck())<<8;
	mag_raw_x |= i2c_readAck();

	mag_raw_z = ((uint8_t)i2c_readAck())<<8;
	mag_raw_z |= i2c_readAck();

	mag_raw_y = ((uint8_t)i2c_readAck())<<8;
	mag_raw_y |= i2c_readNak();

	fixed_x = k1x * (mag_raw_x - k2x);
	fixed_y = k1y * (mag_raw_y - k2y);
	
	i2c_stop();
	
	float heading = atan2(fixed_y,fixed_x);
	float heading_dec = heading+DECLINATION;
	
	
	if(heading < 0)
		heading += 2*PI;
	
	if(heading>2*PI)
		heading-=2*PI;
		
	if(heading_dec < 0)
	heading_dec += 2*PI;
	
	if(heading_dec>2*PI)
	heading_dec-=2*PI;
	
		
	float heading_degrees = heading * 180 / PI;
	float heading_dec_degrees = heading_dec * 180 / PI;
	
	/*
	debug_print("Raw x: ");
	debug_printf(mag_raw_x);
	debug_print("\t Heading (radians): ");
	debug_printf(heading);
	debug_print("\t Heading (degrees): ");
	debug_printf(heading_degrees);
	debug_print("\t Heading (w/Declination): ");
	debug_printf(heading_dec_degrees);
	debug_println("");
	*/
	
	
	return heading_degrees;
}


int16_t getHeadingTiltCompensated(void)
{

	i2c_start_wait(HMC5883L_WRITE);
	i2c_write(0x03); //set pointer to X-axis MSB
	i2c_stop();

	i2c_rep_start(HMC5883L_READ);

	mag_raw_x = ((uint8_t)i2c_readAck())<<8;
	mag_raw_x |= i2c_readAck();

	mag_raw_z = ((uint8_t)i2c_readAck())<<8;
	mag_raw_z |= i2c_readAck();

	mag_raw_y = ((uint8_t)i2c_readAck())<<8;
	mag_raw_y |= i2c_readNak();
	
	
	/* stack variables */
	/* mag_raw_x, mag_raw_y, mag_raw_z: the three components of the magnetometer sensor */
	/* accel_raw_x, accel_raw_y, accel_raw_z: the three components of the accelerometer sensor */
	/* local variables */
	int16_t iSin, iCos; /* sine and cosine */
	/* subtract the hard iron offset */
	mag_raw_x -= iVx; /* see Eq 16 */
	mag_raw_y -= iVy; /* see Eq 16 */
	mag_raw_z -= iVz; /* see Eq 16 */
	
	/* calculate current roll angle Phi */
	iPhi = iHundredAtan2Deg(accel_raw_y, accel_raw_z);/* Eq 13 */
	
	/* calculate sin and cosine of roll angle Phi */
	iSin = iTrig(accel_raw_y, accel_raw_z); /* Eq 13: sin = opposite / hypotenuse */
	iCos = iTrig(accel_raw_z, accel_raw_y); /* Eq 13: cos = adjacent / hypotenuse */
	
	/* de-rotate by roll angle Phi */
	iBfy = (int16_t)((mag_raw_y * iCos - mag_raw_z * iSin) >> 15);/* Eq 19 y component */
	mag_raw_z = (int16_t)((mag_raw_y * iSin + mag_raw_z * iCos) >> 15);/* Bpy*sin(Phi)+Bpz*cos(Phi)*/
	accel_raw_z = (int16_t)((accel_raw_y * iSin + accel_raw_z * iCos) >> 15);/* Eq 15 denominator */
	
	/* calculate current pitch angle Theta */
	iThe = iHundredAtan2Deg((int16_t)-accel_raw_x, accel_raw_z);/* Eq 15 */
	
	/* restrict pitch angle to range -90 to 90 degrees */
	if (iThe > 9000) iThe = (int16_t) (18000 - iThe);
	if (iThe < -9000) iThe = (int16_t) (-18000 - iThe);
	
	/* calculate sin and cosine of pitch angle Theta */
	iSin = (int16_t)-iTrig(accel_raw_x, accel_raw_z); /* Eq 15: sin = opposite / hypotenuse */
	iCos = iTrig(accel_raw_z, accel_raw_x); /* Eq 15: cos = adjacent / hypotenuse */
	
	/* correct cosine if pitch not in range -90 to 90 degrees */
	if (iCos < 0) iCos = (int16_t)-iCos;
	
	/* de-rotate by pitch angle Theta */
	iBfx = (int16_t)((mag_raw_x * iCos + mag_raw_z * iSin) >> 15); /* Eq 19: x component */
	iBfz = (int16_t)((-mag_raw_x * iSin + mag_raw_z * iCos) >> 15);/* Eq 19: z component */
	
	/* calculate current yaw = e-compass angle Psi */
	iPsi = iHundredAtan2Deg((int16_t)-iBfy, iBfx); /* Eq 22 */
	
	return iPsi;
}

const uint16_t MINDELTATRIG = 1; /* final step size for iTrig */

/* function to calculate ir = ix / sqrt(ix*ix+iy*iy) using binary division */
int16_t iTrig(int16_t ix, int16_t iy)
{
	uint32_t itmp; /* scratch */
	uint32_t ixsq; /* ix * ix */
	int16_t isignx; /* storage for sign of x. algorithm assumes x >= 0 then corrects later */
	uint32_t ihypsq; /* (ix * ix) + (iy * iy) */
	int16_t ir; /* result = ix / sqrt(ix*ix+iy*iy) range -1, 1 returned as signed Int16 */
	int16_t idelta; /* delta on candidate result dividing each stage by factor of 2 */
	/* stack variables */
	/* ix, iy: signed 16 bit integers representing sensor reading in range -32768 to 32767 */
	/* function returns signed Int16 as signed fraction (ie +32767=0.99997, -32768=-1.0000) */
	/* algorithm solves for ir*ir*(ix*ix+iy*iy)=ix*ix */
	/* correct for pathological case: ix==iy==0 */
	if ((ix == 0) && (iy == 0)) ix = iy = 1;
	/* check for -32768 which is not handled correctly */
	if (ix == -32768) ix = -32767;
	if (iy == -32768) iy = -32767;
	/* store the sign for later use. algorithm assumes x is positive for convenience */
	isignx = 1;
	if (ix < 0)
	{
		ix = (int16_t)-ix;
		isignx = -1;
	}
	/* for convenience in the boosting set iy to be positive as well as ix */
	iy = (int16_t) abs(iy);
	/* to reduce quantization effects, boost ix and iy but keep below maximum signed 16 bit */
	while ((ix < 16384) && (iy < 16384))
	{
		ix = (int16_t)(ix + ix);
		iy = (int16_t)(iy + iy);
	}
	/* calculate ix*ix and the hypotenuse squared */
	ixsq = (uint32_t)(ix * ix); /* ixsq=ix*ix: 0 to 32767^2 = 1073676289 */
	ihypsq = (uint32_t)(ixsq + iy * iy); /* ihypsq=(ix*ix+iy*iy) 0 to 2*32767*32767=2147352578 */
	/* set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* set as 2^14 = 0.5 */
	/* loop over binary sub-division algorithm */
	do
	{
		/* generate new candidate solution for ir and test if we are too high or too low */	
		/* itmp=(ir+delta)^2, range 0 to 32767*32767 = 2^30 = 1073676289 */
		itmp = (uint32_t)((ir + idelta) * (ir + idelta));
		/* itmp=(ir+delta)^2*(ix*ix+iy*iy), range 0 to 2^31 = 2147221516 */
		itmp = (itmp >> 15) * (ihypsq >> 15);
		if (itmp <= ixsq) ir += idelta;
		idelta = (int16_t)(idelta >> 1); /* divide by 2 using right shift one bit */
	} while (idelta >= MINDELTATRIG); /* last loop is performed for idelta=MINDELTATRIG */
	
	/* correct the sign before returning */
	return (int16_t)(ir * isignx);
}

int16_t iHundredAtan2Deg(int16_t iy, int16_t ix)
{
	int16_t iResult; /* angle in degrees times 100 */
	
	/* check for -32768 which is not handled correctly */
	if (ix == -32768) ix = -32767;
	if (iy == -32768) iy = -32767;
	
	/* check for quadrants */
	if ((ix >= 0) && (iy >= 0)) /* range 0 to 90 degrees */
	{
		iResult = iHundredAtanDeg(iy, ix);
	}
	else if ((ix <= 0) && (iy >= 0)) /* range 90 to 180 degrees */
	{
		iResult = (int16_t)(18000 - (int16_t)iHundredAtanDeg(iy, (int16_t)-ix));
	}
	else if ((ix <= 0) && (iy <= 0)) /* range -180 to -90 degrees */
	{
		iResult = (int16_t)((int16_t)-18000 + iHundredAtanDeg((int16_t)-iy, (int16_t)-ix));
	}
	else /* ix >=0 and iy <= 0 giving range -90 to 0 degrees */
	{
		iResult = (int16_t)(-iHundredAtanDeg((int16_t)-iy, ix));
	}
	
	return (iResult);
}

/* fifth order of polynomial approximation giving 0.05 deg max error */
const int16_t K1 = 5701;
const int16_t K2 = -1645;
const int16_t K3 = 446;


/* calculates 100*atan(iy/ix) range 0 to 9000 for all ix, iy positive in range 0 to 32767 */ 
int16_t iHundredAtanDeg(int16_t iy, int16_t ix)
{
	int32_t iAngle; /* angle in degrees times 100 */
	int16_t iRatio; /* ratio of iy / ix or vice versa */
	int32_t iTmp; /* temporary variable */
	
	/* check for pathological cases */
	if ((ix == 0) && (iy == 0)) return (0);
	if ((ix == 0) && (iy != 0)) return (9000);
	
	/* check for non-pathological cases */
	if (iy <= ix)
	{
		iRatio = iDivide(iy, ix); /* return a fraction in range 0. to 32767 = 0. to 1. */
	}
	else
	{
		iRatio = iDivide(ix, iy); /* return a fraction in range 0. to 32767 = 0. to 1. */
	}
	
	/* first, third and fifth order polynomial approximation */
	iAngle = (int16_t) K1 * (int16_t) iRatio;
	iTmp = ((int16_t) iRatio >> 5) * ((int16_t) iRatio >> 5) * ((int16_t) iRatio >> 5);
	iAngle += (iTmp >> 15) * (int16_t) K2;
	iTmp = (iTmp >> 20) * ((int16_t) iRatio >> 5) * ((int16_t) iRatio >> 5);
	iAngle += (iTmp >> 15) * (int16_t) K3;
	iAngle = iAngle >> 15;
	
	/* check if above 45 degrees */
	if (iy > ix) iAngle = (int16_t)(9000 - iAngle);
	
	/* for tidiness, limit result to range 0 to 9000 equals 0.0 to 90.0 degrees */
	if (iAngle < 0) iAngle = 0;
	if (iAngle > 9000) iAngle = 9000;
	
	return (int16_t) iAngle;
}

uint16_t MINDELTADIV = 1; /* final step size for iDivide */

/* function to calculate ir = iy / ix with iy <= ix, and ix, iy both > 0 */ 
int16_t iDivide(int16_t iy, int16_t ix)
{
	int16_t itmp; /* scratch */
	int16_t ir; /* result = iy / ix range 0., 1. returned in range 0 to 32767 */
	int16_t idelta; /* delta on candidate result dividing each stage by factor of 2 */
	/* set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* set as 2^14 = 0.5 */
	/* to reduce quantization effects, boost ix and iy to the maximum signed 16 bit value */
	while ((ix < 16384) && (iy < 16384))
	{
		ix = (int16_t)(ix + ix);
		iy = (int16_t)(iy + iy);
	}
	/* loop over binary sub-division algorithm solving for ir*ix = iy */
	do
	{
		/* generate new candidate solution for ir and test if we are too high or too low */
		itmp = (int16_t)(ir + idelta); /* itmp=ir+delta, the candidate solution */
		itmp = (int16_t)((itmp * ix) >> 15);
		if (itmp <= iy) ir += idelta;
		idelta = (int16_t)(idelta >> 1); /* divide by 2 using right shift one bit */
	} while (idelta >= MINDELTADIV); /* last loop is performed for idelta=MINDELTADIV */
		
	return (ir);
}