/*
 * gps.cpp
 *
 * Created: 12/09/2014 9:21:25 AM
 *  Author: Chris Barr
 */ 

#import "gps.h"


float gps_conv_coords(float in_coords)
{
	//Initialize the location.
	float f = in_coords;
	// Get the first two digits by turning f into an integer, then doing an integer divide by 100;
	// firsttowdigits should be 77 at this point.
	int firsttwodigits = ((int)f)/100;                               //This assumes that f < 10000.
	float nexttwodigits = f - (float)(firsttwodigits*100);
	float theFinalAnswer = (float)(firsttwodigits + nexttwodigits/60.0);
	return theFinalAnswer;
}