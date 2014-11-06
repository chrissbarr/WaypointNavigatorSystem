////////////////////////////////////////////////////
/*
 * move1.cpp
 *
 * Created: 1/10/2014 9:15:44 PM
 *  Author: mega
 */ 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

#include "main.h"
#include "debug.h"
#include "usart.h"
#include "waypoint.h"
#include "gps.h"
#include "usart.h"

float timeUTC;
float lat1_deg; // start
float lon1_deg; // start
float lat1; // start
float lon1; // start
float lat2_deg; // current
float lon2_deg; // current
float lat2; // current
float lon2; // current
float lat3_deg; // next
float lon3_deg; // next
float lat3; // next
float lon3; //next
float sats;
float gps_altitude;
float dist_to_next;
float bearing_to_next;
float percent_next;

char gpsd;

WayPoint wps[MAX_WAYPOINTS];

int numwps = 0;
int i = 0;
int j = i+1;
int *ptr_j = &j;
int loop1 = 1;
int loop2 = 1;
int chk;
int len;
int fix;

int rc = 0; // read count
int index_waypoint = 0; // way point index
int wyp_rch = 1; // 2m radius to way point
int wyp_reached; // is distance less than wyp_rch: 0 = no, 1 = yes
int slow_radius = 10; // 10% dist remaining
int slow_down; // is distance less than 10% remaining: 0 = no, 1 = yes

char *ptr_gpgga = "GPGGA";
char *space = " ";
char *ptr_start = "Start Position is: ";
char *ptr_invalid = "Invalid signal";
char *ptr_valid = "Valid signal";
char *ptr_finished = "Finished";

char header[DATA_TYPE];
char *ptr_header = header;
char every[EVERYTHING];
char *ptr_every = every;
char nevery[EVERYTHING];
char *ptr_nevery = nevery;

unsigned char ccmpr = 0x24; // $ character
unsigned char ccmpr2 = 0xA; // new line character
unsigned char chop = 0x00; // null character


bool gps_init()
{
	
	USART_init(USART_GPS,9600);
	debug_println("GPS Init...");
	//Now, we wait until we have more than x satellites!
	int consecutive_sats = 0;
	
	do {
		gps_update();
		debug_print("Waiting for satellites...(");
		debug_printi(gps_get_sats());
		debug_println(")");
		if(gps_get_sats()!=0)
			consecutive_sats+=1;
		else
			consecutive_sats=0;
			
	} while (gps_get_sats() < 5 && consecutive_sats>10);
	
	return true;
}

bool gps_update()
{
	bool success = false;
	//debug_println("gps_update start");
	do
	{
		gpsd = USART_receive(USART_GPS);
		if(gpsd == ccmpr) {
			for (i = 0; i < SENTENCE_TYPE; i++) {
				header[i] = USART_receive(USART_GPS);
				//debug_println("$ found!");
			}
			
			
			/*for (int l = 0; l < 5; l++)
			{
				USART_send(USART_PC,header[l]);
			}*/
			
			chk = strcmp("GPGGA",ptr_header);
			if(chk == 0) {
				
				for (i = 0; i < EVERYTHING; i++) {
					every[i] = USART_receive(USART_GPS);
				}
				debug_println("GPGGA Header found, reading data...");
				//--------------------------------------------------------------------
				i = 0;
				while(every[i] != ccmpr2) {
					nevery[i] = every[i];
					i++;
				}
				nevery[i] = chop;
				len = strlen(nevery);
				i = 0; j = i+1;
				timeUTC = get_time(ptr_nevery, ptr_j);
				float lat2_dist = get_lat_dist(ptr_nevery);
				float lon2_dist = get_lon_dist(ptr_nevery);
				lat2_deg = get_lat_deg(ptr_nevery);
				lon2_deg = get_lon_deg(ptr_nevery);
				lat2 = get_lat(ptr_nevery, ptr_j);
				lon2 = get_lon(ptr_nevery, ptr_j);
				fix = get_fix(ptr_nevery, ptr_j);
				sats = get_sats(ptr_nevery, ptr_j);
				success = true;
				debug_println("GPS values saved!");
			}
		}
	} while (chk!=0);
	
	return success;
}


float get_time(char* nev, int* p_j) {
	float tf;
	char t[TIME_TYPE];
	char *ptr_t = t;

	for(int i = 0; i<TIME_TYPE; i++) {
		t[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	tf = atof (ptr_t);
	return(tf);
}


float get_lat_dist(char* nev) {
	float lf;
	char la[LAT_TYPE];
	char *ptr_la = la;
	
	la[0]=nev[11];
	la[1]=nev[12];
	la[2]=nev[13];
	la[3]=nev[14];
	la[4]=nev[16];
	la[5]=nev[17];
	la[6]=nev[18];
	la[7]=0x00;
	lf = atof (ptr_la);
	
	return(lf);
}

float get_lat_deg(char* nev) {
	float lf;
	char la[LAT_TYPE];
	char *ptr_la = la;
	
	la[0]=nev[11];
	la[1]=nev[12];
	la[2]=nev[15];
	la[3]=nev[13];
	la[4]=nev[14];
	la[5]=nev[16];
	la[6]=nev[17];
	la[7]=nev[18];
	la[8]=0x00;
	lf = atof (ptr_la);
	
	return(lf);
}

float get_lat(char* nev, int* p_j) {
	int i;
	float lf;
	char la[LAT_TYPE];
	char *ptr_la = la;
	char temp1;
	char temp2;

	*p_j = *p_j + 1;
	for(i=0; i<LAT_TYPE;i++) {
		la[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	temp1 = la[2];temp2 = la[3];
	la[2] = la[4];la[3] = temp1;la[4] = temp2;
	lf = atof (ptr_la);
	return(lf);
}


float get_lon_dist(char* nev) {
	float lnf;
	char ln[LON_TYPE];
	char *ptr_ln = ln;
	
	ln[0]=nev[24];
	ln[1]=nev[25];
	ln[2]=nev[26];
	ln[3]=nev[27];
	ln[4]=nev[28];
	ln[5]=nev[30];
	ln[6]=nev[31];
	ln[7]=nev[32];
	ln[8]=0x00;
	lnf = atof (ptr_ln);
	
	return(lnf);
}

float get_lon_deg(char* nev) {
	float lnf;
	char ln[LON_TYPE];
	char *ptr_ln = ln;
	
	ln[0]=nev[25];
	ln[1]=nev[26];
	ln[2]=nev[29];
	ln[3]=nev[27];
	ln[4]=nev[28];
	ln[5]=nev[30];
	ln[6]=nev[31];
	ln[7]=nev[32];
	ln[8]=0x00;
	lnf = atof (ptr_ln);
	
	return(lnf);
}

float get_lon(char* nev, int* p_j) {
	int i;
	float lnf;
	char ln[LON_TYPE];
	char *ptr_ln = ln;
	char temp1;
	char temp2;

	*p_j = *p_j + 3;
	for(i=0; i<LON_TYPE;i++) {
		ln[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	temp1 = ln[3];temp2 = ln[4];
	ln[3] = ln[5];ln[4] = temp1;ln[5] = temp2;
	lnf = atof (ptr_ln);
	return(lnf);
}


int get_fix(char* nev, int* p_j) {
	int i;
	int f;
	char fx[FIX_TYPE];
	char *ptr_fx = fx;

	*p_j = *p_j + 3;
	for(i=0; i<FIX_TYPE;i++) {
		fx[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	f = atoi (ptr_fx);
	return(f);
}

int get_sats(char* nev, int* p_j) {
	int i;
	float s;
	char sts[SATS_TYPE];
	char *ptr_sts = sts;

	//*p_j = *p_j + 1;
	for(i=0; i<SATS_TYPE;i++) {
		sts[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	s = atof (ptr_sts);
	return(s);
}

float get_alt(char* nev, int* p_j) {
	int i;
	float alt;
	char a[ALT_TYPE];
	char *ptr_a = a;

	*p_j = *p_j + 4;
	for(i=0; i<ALT_TYPE;i++) {
		a[i] = nev[*p_j];
		*p_j = *p_j + 1;
	}
	alt = atof (ptr_a);
	return(alt);
}

///*debug_printf(timeUTC);debug_print(space);debug_printf(lat2);debug_print(space);debug_printf(lon2);debug_print(space);/*debug_printi(fix);debug_print(space);*/
///*debug_printf(altitude);debug_print(ptr_gps);*///USART_putstring(USART_PC,"\n\r");


//Getters for external functions
float gps_get_latitude(){
	return lat2_deg;
}

float gps_get_longitude()
{
	return lon2_deg;
}

int gps_get_sats()
{
	return sats;
}