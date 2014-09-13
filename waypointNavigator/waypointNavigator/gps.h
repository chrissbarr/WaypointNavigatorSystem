/*
 * gps.h
 *
 * Created: 12/09/2014 9:22:37 AM
 *  Author: Chris Barr
 */ 

//-------------GPS INPUT FUNCTION PROTOTYPES---------------//
void gps_init();			//initializes GPS system, begins communication
bool gps_locked();			//checks if GPS is locked and data is available
void gps_update_values();	//gets data from GPS and stores relevant data in variables

float gps_conv_coords(float in_coords);