/*
 * waypoint.cpp
 *
 * Created: 8/09/2014 9:00:36 PM
 *  Author: Stephen John Weatherley
 *	Modified: Chris Barr
 */ 

#include "waypoint.h"
#include "debug.h"
#include "main.h"

int chris_array_index = 0;
float c_lat_array[NUM_WAYPOINTS];
float c_long_array[NUM_WAYPOINTS];

/////////////////////////////////////////////////////////////////////////////////
// function definitions
int user_waypoint_test (WayPoint wps[],int numwps) {
	int i;
	int uj = numwps + 1;
	int uloop1 = 1;
	int userchk1;
	int userchk2;

	float ulat;
	float ulon;
	float ulat_d;
	float ulat_dist;
	float ulon_dist;
	float ulon_d;
		
	char entwp[3];
	char *ptr_entwp = entwp;
	
	char entlat[ULAT_TYPE];
	char *ptr_entlat = entlat;
	char entlat_dist[ULAT_TYPE];
	char *ptr_entlat_dist = entlat_dist;
	char entlon[ULONG_TYPE];
	char *ptr_entlon = entlon;
	char entlon_dist[ULONG_TYPE];
	char *ptr_entlon_dist = entlon_dist;
	char entlon_d[ULONG_TYPE];
	char *ptr_e_d = entlon_d;
	
	char yes[3] = "y";
	char *ptr_yes = yes;
	char no[3] = "n";
	char *ptr_no = no;
	
	char *ptr_1 = "Please enter Latitude ";
	char *ptr_2 = ":";
	char *ptr_3 = "Please enter Longitude ";
	char *ptr_4 = "Would you like to enter another way point? (y or n):";
	char *ptr_6 = "Finished entering way points";
	char *ptr_7 = "Invalid, please try again";
	char *ptr_8 = " ";
	
	// user enter first lat
	USART_putstring(USART_PC,"\n\r"); debug_print(ptr_1); debug_printi(uj); debug_print(ptr_2);USART_putstring(USART_PC,"\n\r");
	for (i = 0; i < UENTLAT; i++) {
		entlat[i] = USART_receive(USART_PC);
	}
	/*entlat_dist[0]=entlat[0];
	entlat_dist[1]=entlat[1];
	entlat_dist[2]=entlat[3];
	entlat_dist[3]=entlat[4];
	entlat_dist[4]=entlat[5];
	entlat_dist[5]=entlat[6];
	entlat_dist[6]=entlat[7];
	entlat_dist[7]=0x00;*/
	
	ulat = atof (ptr_entlat);
	//ulat_dist = atof (ptr_entlat_dist);
	ulat_d = ulat;

	debug_printf(ulat);debug_print(ptr_8);debug_printf(ulat_d);
	wps[numwps].latitude = ulat;
	//wps[numwps].latitude_dist = ulat_dist;
	wps[numwps].latitude_deg = ulat_d;
	
	// user enter first long
	USART_putstring(USART_PC,"\n\r"); debug_print(ptr_3); debug_printi(uj); debug_print(ptr_2);USART_putstring(USART_PC,"\n\r");
	for (i = 0; i < UENTLON; i++) {
		entlon[i] = USART_receive(USART_PC);
	}
	/*entlon_dist[0]=entlon[0];
	entlon_dist[1]=entlon[1];
	entlon_dist[2]=entlon[2];
	entlon_dist[3]=entlon[4];
	entlon_dist[4]=entlon[5];
	entlon_dist[5]=entlon[6];
	entlon_dist[6]=entlon[7];
	entlon_dist[7]=entlon[8];
	entlon_dist[8]=0x00;*/
	
	entlon_d[0]=entlon[1];
	entlon_d[1]=entlon[2];
	entlon_d[2]=entlon[3];
	entlon_d[3]=entlon[4];
	entlon_d[4]=entlon[5];
	entlon_d[5]=entlon[6];
	entlon_d[6]=entlon[7];
	entlon_d[7]=entlon[8];
	entlon_d[8]=0x00;
		
	ulon = atof (ptr_entlon);
	//ulon_dist = atof (ptr_entlon_dist);
	ulon_d = atof (ptr_e_d);
	
	debug_printf(ulon);debug_print(ptr_8);debug_printf(ulon_d);
	wps[numwps].longitude = ulon;
	//wps[numwps].longitude_dist = ulon_dist;
	wps[numwps].longitude_deg = ulon_d;
	//-----------------------------------------------------------
	
	numwps = numwps + 1;
	
	// prompt if user wants to enter another way point
	USART_putstring(USART_PC,"\n\r"); debug_println(ptr_4);
	for (i = 0; i < 1; i++) {
		entwp[i] = USART_receive(USART_PC);
	}
	entwp[1]=0x00;
	debug_println(ptr_entwp);

	while (uloop1 == 1) {
		
		userchk1 = strcmp(ptr_entwp,ptr_yes);
		userchk2 = strcmp(ptr_entwp,ptr_no);
		
		if(userchk1 == 0) {
			numwps = userwaypoint(wps,numwps);
			USART_putstring(USART_PC,"\n\r");debug_println(ptr_4);
			for (i = 0; i < 1; i++) {
				entwp[i] = USART_receive(USART_PC);
			}
			debug_println(ptr_entwp);
		}
		else if (userchk2 == 0) {
			debug_println(ptr_6);
			uloop1 = 0;
		}
		else {
			debug_println(ptr_7); USART_putstring(USART_PC,"\n\r");debug_println(ptr_4);
			for (i = 0; i < 1; i++) {
				entwp[i] = USART_receive(USART_PC);
			}
			debug_println(ptr_entwp);
		}
	}
return(numwps);
}

int userwaypoint(WayPoint wps[],int nwps) {
	int i = nwps;
	int j;
	int k;

	float uulat;
	float uulat_dist;
	float uulat_d;
	float uulon;
	float uulon_dist;
	float uulon_d;
	
	char entlat[ULAT_TYPE];
	char *ptr_entlat = entlat;
	char entlat_dist[ULAT_TYPE];
	char *ptr_entlat_dist = entlat_dist;
	char entlon[ULONG_TYPE];
	char *ptr_entlon = entlon;
	char entlon_dist[ULONG_TYPE];
	char *ptr_entlon_dist = entlon_dist;
	char entlon_d[ULONG_TYPE];
	char *ptr_e_d = entlon_d;
	
	char *ptr_1 = "Please enter Latitude ";
	char *ptr_2 = ":";
	char *ptr_3 = "Please enter Longitude ";
	char *ptr_4 = " ";
	
	j = i + 1;
	
	USART_putstring(USART_PC,"\n\r"); debug_print(ptr_1); debug_printi(j); debug_print(ptr_2);USART_putstring(USART_PC,"\n\r");
	for (k = 0; k < UENTLAT; k++) {
		entlat[k] = USART_receive(USART_PC);
	}
	//-----------------------------------------------------------
	/*entlat_dist[0]=entlat[0];
	entlat_dist[1]=entlat[1];
	entlat_dist[2]=entlat[3];
	entlat_dist[3]=entlat[4];
	entlat_dist[4]=entlat[5];
	entlat_dist[5]=entlat[6];
	entlat_dist[6]=entlat[7];
	entlat_dist[7]=0x00;*/
	
	uulat = atof (ptr_entlat);
	//uulat_dist = atof (ptr_entlat_dist);
	uulat_d = uulat;
	
	debug_printf(uulat);debug_print(ptr_4);debug_printf(uulat_d);
	wps[i].latitude = uulat;	
	//wps[i].latitude_dist = uulat_dist;
	wps[i].latitude_deg = uulat;
	//-----------------------------------------------------------
		
	USART_putstring(USART_PC,"\n\r"); debug_print(ptr_3); debug_printi(j); debug_print(ptr_2);USART_putstring(USART_PC,"\n\r");
	for (k = 0; k < UENTLON; k++) {
		entlon[k] = USART_receive(USART_PC);
	}
	//-----------------------------------------------------------
	/*entlon_dist[0]=entlon[0];
	entlon_dist[1]=entlon[1];
	entlon_dist[2]=entlon[2];
	entlon_dist[3]=entlon[4];
	entlon_dist[4]=entlon[5];
	entlon_dist[5]=entlon[6];
	entlon_dist[6]=entlon[7];
	entlon_dist[7]=entlon[8];
	entlon_dist[8]=0x00;*/
	
	entlon_d[0]=entlon[1];
	entlon_d[1]=entlon[2];
	entlon_d[2]=entlon[3];
	entlon_d[3]=entlon[4];
	entlon_d[4]=entlon[5];
	entlon_d[5]=entlon[6];
	entlon_d[6]=entlon[7];
	entlon_d[7]=entlon[8];
	entlon_d[8]=0x00;
		
	uulon = atof (ptr_entlon);
	//uulon_dist = atof (ptr_entlon_dist);
	uulon_d = atof (ptr_e_d);
	
	debug_printf(uulon);debug_print(ptr_4);debug_printf(uulon_d);
	wps[i].longitude = uulon;
	//wps[i].longitude_dist = uulon_dist;
	wps[i].longitude_deg = uulon_d;
	//-----------------------------------------------------------
	i = i + 1;

return(i);
}

void printwaypoint(WayPoint wps[], int nwps) {
	int i;
	int j = 1;

	i = nwps;
	
	char *ptr_1 = "Summary";
	char *ptr_2 = ("=========================");
	char *ptr_3 = ("End Summary");
	char *ptr_4 = "   ";
	
	USART_putstring(USART_PC,"\n\r"); debug_println(ptr_1); debug_println(ptr_2);
		for (i = 0; i < nwps; i++) {
			debug_printf(wps[i].latitude);debug_print(ptr_4);/*debug_printf(wps[i].latitude_dist);debug_print(ptr_4);*/debug_printf(wps[i].latitude_deg);debug_print(ptr_4);debug_printf(wps[i].longitude);/*debug_print(ptr_4);debug_printf(wps[i].longitude_dist);*/debug_print(ptr_4);debug_printf(wps[i].longitude_deg);
			USART_putstring(USART_PC,"\n\r");
			j = j + 1;
		}
	debug_println(ptr_2); debug_println(ptr_3); USART_putstring(USART_PC,"\n\r");
}

// function for degrees to radians
float deg_to_rad(float degrees) {
	return(degrees * PI / 180);
}

// function for radians to degrees
float rad_to_deg(float rad) {
	return(rad * 180 / PI);
}

// function for wp difference
float difference(float num1, float num2) {
	return(num1-num2);
}

// function for distance
//returns the straight-line distance between two coordinates using haversine formula to (hopefully) overcome limited accuracy of float
float waypoint_get_distance(float num1, float num2, float num3, float num4) {
	float a = pow((sin(difference(deg_to_rad(num3),deg_to_rad(num1))/2)),2) + cos(num3) * cos(num1) * pow((sin(difference(deg_to_rad(num4),deg_to_rad(num2))/2)),2);
	float c = 2 * atan2(sqrt(a),sqrt(1-a));

	return (EARTH_R * c);
}

float waypoint_get_bearing(float num1, float num2, float num3, float num4) {
	float dividend;
	float divisor;
	
	dividend = atan2((sin(num4-num2)*cos(num3)),(cos(num1)*sin(num3)-(sin(num1)*cos(num3)*cos(num4-num2))));
	divisor = 2*PI;
	return(rad_to_deg(dividend - divisor * floor (dividend / divisor)));
}

// function for check if waypoint reached
int location_check (float number1, int number2) {
  int result;
	
  if (number1 < number2) {
	result = 1;
  }
  else {
	  result = 0;
  }
  return (result);
}

// function to increment waypoint index
int index_check (int num1, int num2, int *reset_num1) {
	if (num1 == 1) {
		num2 = num2 + 1;
		*reset_num1 = 0;
	}
	return (num2);
}

float check_percent(float num1, float num2, float num3, float num4, float num5) {
	float total_dist;
	
	total_dist = waypoint_get_distance(num1, num2, num3, num4);

	return (num5 / total_dist * 100);
}

int slow_flight (float num1, int num2) {
	int slow;
	
	if (num1 < num2) {
		slow = 1;
	}
	else {
		slow = 0;
	}
	return (slow);
}




//tools we need
double deg_to_rad(double degrees) {
	return(degrees * PI / 180);
}

double rad_to_deg(double rad) {
	return(rad * 180 / PI);
}

double difference(double num1, double num2)
{
	return(num1-num2);
}



//getter for desired heading
float waypoint_get_heading()
{
	//waypoint_get_bearing(gps_get_latitude(),gps_get_longitude(),)
	return 999;
}


//Chris's last minute demo stuff
void chris_waypoint_init()
{
	do 
	{
		gps_update();
	} while (gps_get_latitude()==0 || gps_get_longitude()==0);
	
	float center_lat = gps_get_latitude();
	float center_long = gps_get_longitude();
	float circle_scale = 10000;	//100 = radius of ~1000m, 1000 = radius of ~100m, 4000 = radius of ~ 25m
	
	debug_print("Generating circle of waypoints around center latitude ");
	debug_printf(center_lat);
	debug_print(", longitude ");
	debug_printf(center_lat);
	debug_println("...");
	for(int i = 0; i < NUM_WAYPOINTS; i++)
	{
		float temp_lat, temp_long;
		
		temp_lat = center_lat-cos((2*PI*i)/NUM_WAYPOINTS)/circle_scale;
		temp_long = center_long+sin((2*PI*i)/NUM_WAYPOINTS)/circle_scale;
		chris_waypoint_add(temp_lat,temp_long);
		debug_printf(temp_lat);
		debug_print(", ");
		debug_printf(temp_long);
		debug_println("");
	}
	
	chris_array_index=0;

}
void chris_waypoint_add(float lat, float lon)
{
	c_lat_array[chris_array_index] = lat;
	c_long_array[chris_array_index] = lon;
	chris_array_index++;
}

//check if we're at the waypoint, if so, move on
void chris_waypoint_update()
{
	if(chris_waypoint_current_distance()<1)
		chris_array_index++;
	
	if(chris_waypoint_current_index()>NUM_WAYPOINTS)
		chris_set_waypoint_current_index(0);
}

//return the bearing to the next waypoint
float chris_waypoint_current_bearing()
{
	//return waypoint_get_bearing(gps_get_latitude(),gps_get_longitude(),c_lat_array[chris_array_index],c_long_array[chris_array_index]);
	return waypoint_get_bearing(c_lat_array[chris_array_index],c_long_array[chris_array_index],gps_get_latitude(),gps_get_longitude());
}

//return the distance to the next waypoint
float chris_waypoint_current_distance()
{
	return waypoint_get_distance(gps_get_latitude(),gps_get_longitude(),c_lat_array[chris_array_index],c_long_array[chris_array_index]);
}

int chris_waypoint_current_index()
{
	return chris_array_index;
}

void chris_set_waypoint_current_index(int new_index)
{
	chris_array_index = new_index;
}

int chris_get_max_waypoints()
{
	return NUM_WAYPOINTS;
}

float array_get_lat(int index)
{
	return c_lat_array[index];
}

float array_get_lon(int index)
{
	return c_long_array[index];
}