/*
 * gps.cpp
 *
 * Created: 12/09/2014 9:21:25 AM
 *  Author: Stephen John Weatherley
 */ 

#include "gps.h"

int chk1, chk2;
int fix;

float latd;
float longid;
float timeUTC;
float latitude;
float longitude;
float sats;
float course;

char gpsd;
char gpsc5, gpsc6;
char gpslat10;
char gpslong11;
char gpsfix0;
char header[HEADER_TYPE];

char *ptr_gpgga ="GPGGA";
char *ptr_gprmc ="GPRMC";
char *ptr_space = " ";
//char *ptr_test ="test";

unsigned char ccmpr = 0x24; // $ character


bool gps_init()			//initializes GPS system, begins communication
{
	//enable communication with GPS module
	USART_init(USART_GPS,9600);
	USART_init(USART_PC,9600);

	//need some sort of check here

	return true;
}

void gps_update_values()
{
	// check for "$"
	gpsd = USART_receive(USART_GPS);
	
	// $ found
	if(gpsd == ccmpr) 
	{
		// sentence header
		for (int i = 0; i < HEADER_TYPE; i++) 
		{
			header[i] = USART_receive(USART_GPS);
		}
		char *ptr_header = header;
		
		// check for GPGGA
		chk1 = strcmp(ptr_gpgga,ptr_header);
		if(chk1 == 0) 
		{
			// UTC time
			timeUTC = gps_time_func();
			
			// latitude
			latitude = gps_get_lat();
			gpslat10 = USART_receive(USART_GPS);
			
			// longitude
			longitude = gps_get_long();
			gpslong11 = USART_receive(USART_GPS);
			
			// degrees format
			latd = latitude / 100;
			longid = longitude / 100;
			
			// gps fix valid
			gpsc5 = USART_receive(USART_GPS);//,
			gpsfix0 = USART_receive(USART_GPS);
			char *ptr_fix = &gpsfix0;
			fix = atoi (ptr_fix);
			
			// number of sats
			sats = gps_get_sats();
		}
		
		// check for GPRMC
		chk2 = strcmp(ptr_gprmc,ptr_header);
		if(chk2 == 0) 
		{
			course = gps_get_course(); // course
		}
	}
		
}

float gps_conv_coords(float in_coords)
{
	//Initialize the location.
	float f = in_coords;
	// Get the first two digits by turning f into an integer, then doing an integer divide by 100;
	// first two digits should be 77 at this point.
	int firsttwodigits = ((int)f)/100;                               //This assumes that f < 10000.
	float nexttwodigits = f - (float)(firsttwodigits*100);
	float theFinalAnswer = (float)(firsttwodigits + nexttwodigits/60.0);
	return theFinalAnswer;
}

float gps_time_func(void) {
	int i;
	char gps[TIME_TYPE];
	char gpsc0;
	
	gpsc0 = USART_receive(USART_GPS);//,
	for (i = 0; i < TIME_TYPE; i++) {
		gps[i] = USART_receive(USART_GPS);
	}
	char *ptr_time = gps;
	float time = atof (ptr_time);
	return (time);
}

float gps_get_lat(void) {
	int i;
	char gps[LAT_TYPE];
	char gpsc1, gpsc2;
	
	gpsc1 = USART_receive(USART_GPS);//,
	for (i = 0; i < LAT_TYPE; i++) {
		gps[i] = USART_receive(USART_GPS);
	}
	gpsc2 = USART_receive(USART_GPS);//,
	char *ptr_lat = gps;
	float lat = atof (ptr_lat);
	return (lat);
}

float gps_get_long(void) {
	int i;
	char gps[LONG_TYPE];
	char gpsc3, gpsc4;
	
	gpsc3 = USART_receive(USART_GPS);//,
	for (i = 0; i < LONG_TYPE; i++) {
		gps[i] = USART_receive(USART_GPS);
	}
	gpsc4 = USART_receive(USART_GPS);//,
	char *ptr_long = gps;
	float longi = atof (ptr_long);
	return(longi);
}

float gps_get_sats(void) {
	int i;
	char gps[SAT_TYPE];
	char gpsc6;
	
	gpsc6 = USART_receive(USART_GPS);//,
	for (i = 0; i < SAT_TYPE; i++) {
		gps[i] = USART_receive(USART_GPS);
	}
	char *ptr = gps;
	float s = atof (ptr);
	return (s);
}

float gps_get_course(void) {
	char unused[UNUSED];
	char gps[COURSE];
	
	for (int i = 0; i < UNUSED; i++) {
		unused[i] = USART_receive(USART_GPS);
	}
	for (int j = 0; j < COURSE; j++) {
		gps[j] = USART_receive(USART_GPS);
	}
	char *ptr_course = gps;
	float crs = atof (ptr_course);
	crs = crs / 1000;
	return(crs);
}