/*
 * gps.h
 *
 *  Created on: 23 Apr 2025
 *      Author: henri
 */

#ifndef UTILS_GPS_GPS_H_
#define UTILS_GPS_GPS_H_

//libraries
//#include "../comProtocols/Lpuart/lpuart2_interrupt.h"
//#include "../comProtocols/Lpuart/lpuart0_interrupt.h"
//#include <math.h>
//#include <stdlib.h>
//#include <string.h>

//variables defines

#ifdef DEBUG
#define TARGETSTR "Debug"
#else
#define TARGETSTR "Release"
#endif

typedef struct Coordinates
	{
		float lon;
		float lat;
		char lonDir;
		char latDir;
	}coordinates_t;



int gpsInit(void);
float convertToDecimal(float value,char direction);
char * strsep (char **stringp, const char *delim);
void parseNMEA(char buffer[128], coordinates_t *boxCoordinates);
coordinates_t getPosition();
void updatePosition();

#endif /* UTILS_GPS_GPS_H_ */
