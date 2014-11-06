

#define SENTENCE_TYPE 5
#define DATA_TYPE 7
#define TIME_TYPE 9
#define LAT_TYPE 10
#define LON_TYPE 11
#define FIX_TYPE 2
#define SATS_TYPE 4
#define ALT_TYPE 5
#define EVERYTHING 70
#define CORRECT 60

//getters to access from elsewhere
float gps_get_latitude();
float gps_get_longitude();
int gps_get_sats();

//mian functions
bool gps_init();
bool gps_update();
float get_time(char*, int*);
float get_lat_dist(char*);
float get_lat_deg(char*);
float get_lat(char*, int*);
float get_lon_dist(char*);
float get_lon_deg(char*);
float get_lon(char*, int*);
int get_fix(char*, int*);
int get_sats(char*, int*);
float get_alt(char*, int*);