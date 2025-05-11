/*
 * gps.c
 *
 *  Created on: 23 Apr 2025
 *      Author: henri
 */
#include "gps.h"
#include "../source/games/gpsGame/gpsGame.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "../source/utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "../source/utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include <stdio.h>

char buffer[128];
coordinates_t targetCoordinates;
coordinates_t boxCoordinates;


// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------

int gpsInit(void)
{

	targetCoordinates.lon = 6.8845;
	targetCoordinates.lat = 51.4186;
	targetCoordinates.lonDir = 'E';
	targetCoordinates.latDir = 'N';




    lpuart2_init(9600);
    lpuart0_init(9600);


    printf("LPUART0 Interrupt");
    printf(" - %s\r\n", TARGETSTR);
    printf("Build %s %s\r\n", __DATE__, __TIME__);

}

// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------

void updatePosition()
{

	while(lpuart2_rxcnt() > 0)
	    	{
	    	char c = (char)lpuart2_getchar();
	    	strncat(buffer, &c, 1);
			//printf("%c",c);
			if( c == '\n')
			{
				if(strncmp(buffer, "$GNGGA",6) == 0)
				{
					printf("yes\n\n\r");
					parseNMEA(buffer, &boxCoordinates);
					printf("Lon: %lf \n\r", boxCoordinates.lon);
					printf("LonDir: %c \n\r", boxCoordinates.lonDir);
					printf("Lat: %lf \n\r", boxCoordinates.lat);
					printf("LatDir: %c \n\n\r", boxCoordinates.latDir);
					float d = distance(getPosition(), targetCoordinates);
					printf("%lf \n\r", d);
					float b = calculateBearing(getPosition(),targetCoordinates);
					printf("Direction NP: %lf \n", b);
		        	buffer[0] = '\0'; //clear buffer


				}
				else
				{
		        	buffer[0] = '\0'; //clear buffer

				}
			}


}

}




void parseNMEA(char buffer[128], coordinates_t *boxCoordinates)
{

	float lon;
	float lat;
	char latDir;
	char lonDir;
	int nrSat;
	char *ptr = buffer;
	char *token; // Tokenize the NMEA sentence
	int field = 0;
	    while ((token = strsep(&ptr, ",")) != NULL) {
	      //field 0: name, field 1: UTC time
	       switch(field)
	       {
	        case 2:
	        lat = atof(token);

	        break;
	        case 3:
	        latDir = token[0];
	        break;
	        case 4:
	        lon = atof(token);

	        break;
	        case 5:
	        lonDir = token[0];

	        break;
	        case 6:
	        break;
	        case 7:
	        nrSat = atoi(token);
	        break;
	       }

	        //field 6:valid fix, field 7: number of satellites, field 8 horizontal dillution, field 9: altitude, field 10: geoidal seperation magnitude, field 11:geoidal seperation unit, field 12: differential GPS, field 13: stuff for error detection
	        field++;
			token = strsep(&buffer, ",");

	        //token = strtok(NULL, ","); // Move to the next field
	    }
	    if (lat && lon && latDir && lonDir)
	    {
	    	boxCoordinates->lon = convertToDecimal(lon,lonDir);
	    	boxCoordinates->lat = convertToDecimal(lat,latDir);
	    	boxCoordinates->lonDir = lonDir;
	    	boxCoordinates->latDir = latDir;

	    }

}

float convertToDecimal(float value,char direction) { //formula to convert notation from DMM(Degrees and decimal minutes) to decimal degrees
    int degrees = (int)(value / 100);
    float minutes = value - (degrees * 100);
    float decimal = degrees + (minutes / 60.0);

    if (direction == 'S' || direction == 'W') {
        decimal *= -1;
    }

    return decimal;
}

coordinates_t getPosition()
{
	return boxCoordinates;
	}





char * strsep (char **stringp, const char *delim)
{
  char *start = *stringp;
  char *ptr;

  if (start == NULL)
    return NULL;

  if (delim[0] == '\0')
    {
      *stringp = NULL;
      return start;
    }

  if (delim[1] == '\0')
    ptr = strchr (start, delim[0]);
  else
    ptr = strpbrk (start, delim);
  if (ptr == NULL)
    {
      *stringp = NULL;
      return start;
    }

  *ptr = '\0';
  *stringp = ptr + 1;

  return start;
}

