/*
 * waypoint.cpp
 *
 * Created: 8/09/2014 9:00:36 PM
 *  Author: Stephen John Weatherley
 */ 

#include "waypoint.h"

//main waypoint functions
//returns the straight-line distance between two coordinates using haversine formula to (hopefully) overcome limited accuracy of float
float waypoint_get_distance(float lat1, float long1, float lat2, float long2)
{
	float a = square(sin(deg_to_rad(difference(lat2,lat1)/2))) + cos(lat1) * cos(lat2) * square(sin(deg_to_rad(difference(long2,long1))/2));
	float c = 2 * atan2(sqrt(a),sqrt(1-a));
	
	return EARTH_R * c;
}

float waypoint_get_bearing(float lat1, float long1, float lat2, float long2)
{
	return 0;
}



float percentage_travelled(float lat1, float long1, float lat2, float long2, float lat3, float long3)
{
	float distance_travelled = waypoint_get_distance(lat1,long1,lat2,long2);
	float distance_total = waypoint_get_distance(lat1,long1,lat3,long3);
	
	float percent = distance_travelled/distance_total;
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
