#ifndef GPS
#define GPS

#include <stdbool.h>
#include <string.h>
#define M_PI 3.14159265358979323846
#define EARTH_RADIUS 6378137 // From https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html

#define NUM_LANDMARKS 5

typedef struct {
    double latitude;    
    double longitude;  
    char name[32];    
} Landmark;

void GPS_Read(void);
void GPS_Parse(void);
bool GPS_IsFixed(void);
const char* GPS_GetNearestLandmarkName(void);
float ToDegree(float angle);
float ToRad(float angle);
double Distance(float current_lat, float curremt_lon, float destination_lat, float destination_lon);
int findNearestLandmark(float current_lat, float current_lon);
float simple_atof(const char *str) ;
void send_data(void);
#endif
