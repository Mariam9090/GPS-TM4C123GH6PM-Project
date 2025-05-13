#include "../headers/GPS.h"
#include "../headers/UART.h"
#include "../headers/LCD.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <float.h> // Include for DBL_MAX

char GPS_received[80];
char GPS_logname[] = "$GPRMC,";

char formatted_GPS[12][20];
char* token;

float current_lat, current_lon;
bool GPS_fixed = false;


static Landmark landmarks[NUM_LANDMARKS] = {
	{30.06415324674163, 31.280159285564785, "Hall A and B"},
	{30.063425171075185, 31.279606851446, "Luban Workshop"},
	{30.06337125639116, 31.278438707505877, "Credit Building"},
	{30.064098662378914, 31.27884897590333, "Mechanical Workshops"},
	{30.065437399309705, 31.28015826620283, "Library"},
};

float simple_atof(const char *str) {
    if (str == NULL) return 0.0;  
    float result = 0.0;
    float frac = 0.1;
    int is_fraction = 0;

    while (*str) {
        if (*str == '.') {
            is_fraction = 1;
        } else if (*str >= '0' && *str <= '9') {
            if (!is_fraction) {
                result = result * 10 + (*str - '0');
            } else {
                result += (*str - '0') * frac;
                frac *= 0.1;
            }
        }
        str++;
    }
    return result;
}

void GPS_Read(void) {
	char recieved_char;
	bool flag;
    do {
		flag = true;
		for (unsigned char i = 0; i < 7; i++) {
			if (UART_GetChar() != GPS_logname[i]) {
				flag = false;
	
				break;
			}
		}
	} while (!flag);
	strcpy(GPS_received, ""); // Initialize GPS array
	
	unsigned char GPScounter = 0;
	do {
		recieved_char = UART_GetChar();
		GPS_received[GPScounter++] = recieved_char;
		
	} while (recieved_char != '*');
	GPS_Parse();
}

void GPS_Parse(void) {
	unsigned char numberOfTokens = 0;
	token = strtok(GPS_received, ",");
	do {
		strcpy(formatted_GPS[numberOfTokens], token);
		token = strtok(NULL, ",");
		numberOfTokens++;
	} while (token != NULL);

	
		if(strcmp(formatted_GPS[1], "A") == 0) { // Valid GPS data
			GPS_fixed = true;
			if (strcmp(formatted_GPS[3], "N") == 0) { // North is positive
				current_lat = simple_atof(formatted_GPS[2]);
			} else { // South is negative
				current_lat = -simple_atof(formatted_GPS[2]);
			}
			if (strcmp(formatted_GPS[5], "E") == 0) { // East is positive
				current_lon = simple_atof(formatted_GPS[4]);
			} else { // West is negative
				current_lon = -simple_atof(formatted_GPS[4]);
			}
		} else { // Invalid GPS data
			current_lat = 0;
			current_lon = 0;
			GPS_fixed = false;
		  } 
		send_data(); // To arduino
}

void send_data()
{
	print5_str(formatted_GPS[2]);
	print5_str(",");
	print5_str(formatted_GPS[4]);
	print5_str(",");
}


bool GPS_IsFixed(void) {
	return GPS_fixed;
}

const char* GPS_GetNearestLandmarkName(void) {
	if (!GPS_fixed) {
		return "No GPS Signal";
	}
	int nearestIndex = findNearestLandmark(current_lat,current_lon);
	return landmarks[nearestIndex].name;
}


float ToDecimalDegree(float angle) {
	int degree = (int) angle/ 100;
	float minutes = angle - ((float)degree * 100) ;
	return degree + (minutes / 60);
}

float ToRad(float angle) {
	return angle * (M_PI / 180);
}

//Haversine formula
double Distance(float current_lat, float current_lon, float destination_lat, float destination_lon) {
	// Get radian angle
	
	float current_lat_rad = ToRad(ToDecimalDegree(current_lat));
	float current_lon_rad = ToRad(ToDecimalDegree(current_lon));
	float destination_lat_rad = ToRad(destination_lat);
	float destination_lon_rad = ToRad(destination_lon);

	// Get difference between radian angle
	float diff_lat = destination_lat_rad - current_lat_rad;
	float diff_lon = destination_lon_rad - current_lon_rad;
	
	// Get distance
	float a = pow(sin(diff_lat / 2), 2) + cos(current_lat_rad) * cos(destination_lat_rad) * pow(sin(diff_lon / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return c * EARTH_RADIUS;
}

int findNearestLandmark(float current_lat, float current_lon) {
    int nearestIndex = -1;
    double minDistance = DBL_MAX; // Start with a large number

    for (unsigned char i = 0; i < NUM_LANDMARKS; i++) {
      double distance = Distance(current_lat, current_lon, landmarks[i].latitude, landmarks[i].longitude);
      if (distance < minDistance) {
				minDistance = distance;
        nearestIndex = i;
      }
    }
    return nearestIndex;
}