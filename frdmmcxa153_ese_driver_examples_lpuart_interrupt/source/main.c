/*! ***************************************************************************
 *
 * \brief     Main application
 * \file      main.c
 * \author    Hugo Arends
 * \date      February 2024
 *
 * \see       NXP. (2024). MCX A153, A152, A143, A142 Reference Manual. Rev. 4,
 *            01/2024. From:
 *            https://www.nxp.com/docs/en/reference-manual/MCXAP64M96FS3RM.pdf
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include <MCXA153.h>
#include <stdio.h>
#include "../source/utils/gps/gps.h"
//#include "../source/games/gpsGame/gpsGame.h"
#include "../source/utils/comProtocols/GPIO/gpio_output.h"
#include "../source/utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "../source/utils/comProtocols/Lpuart/lpuart0_interrupt.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
// -----------------------------------------------------------------------------
// Local type definitions
// -----------------------------------------------------------------------------
#ifdef DEBUG
#define TARGETSTR "Debug"
#else
#define TARGETSTR "Release"
#endif

#define EARTH_RADIUS 6371000  // Earth rad in meters
const double PI = 3.14159265359;
/*
typedef struct Coordinates
	{
		float lon;
		float lat;
		char lonDir;
		char latDir;
	}coordinates_t;
	*/
typedef struct Direction
{
 	char abreviation[2];
}direction_t;

// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

//float convertToDecimal(float value,char direction);
//void parseNMEA(char buffer[128], coordinates_t *boxCoordinates);
float distance(coordinates_t Loc1, coordinates_t Loc2);
float calculateBearing(coordinates_t loc1, coordinates_t loc2);
float degToRad(float degrees);
float radToDeg(float radians);
char * strsep (char **stringp, const char *delim);

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------

char buffer[128];
coordinates_t boxCoordinates;
coordinates_t targetCoordinates;

// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------
/*
int main(void)
{
//setup
	gpsInit();

	while(1)
	{
		updatePosition();
	}
}
*/

int main(void)
{

	//targetCoordinates.lon = 0;
	//targetCoordinates.lat = 90;
	//targetCoordinates.lonDir = 'E';
	//targetCoordinates.latDir = 'N';




    gpio_output_init();

    gpsInit();
    /*
    lpuart2_init(9600);
    lpuart0_init(9600);


    printf("LPUART0 Interrupt");
    printf(" - %s\r\n", TARGETSTR);
    printf("Build %s %s\r\n", __DATE__, __TIME__);
*/

    while(1)
    {
    	updatePosition(buffer, &boxCoordinates);

/*
    	while(lpuart2_rxcnt > 0)
    	{
    	c = (char)lpuart2_getchar();
    	strncat(buffer, &c, 1);
		printf("%c",c);
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
				//calculateBearing(&boxCoordinates,&targetCoordinates);
				//float d = distance(boxCoordinates,targetCoordinates);
				//printf("Distance NP: %lf \n", d);
				//float b = calculateBearing(boxCoordinates,targetCoordinates);
				//printf("Direction NP: %lf \n", b);
	        	buffer[0] = '\0'; //clear buffer


			}
			else
			{
	        	buffer[0] = '\0'; //clear buffer

			}
		}

    	}

*/
    }


}


// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------
/*
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

	        token = strtok(NULL, ","); // Move to the next field
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
*/
float distance(coordinates_t loc1, coordinates_t loc2) { //Haversine distance formula


	float differenceLat = degToRad(loc2.lat - loc1.lat);
    float differenceLon = degToRad(loc2.lon - loc1.lon);



    float a = sin(differenceLat / 2) * sin(differenceLat / 2) +
               cos(degToRad(loc1.lat)) * cos(degToRad(loc2.lat)) *
               sin(differenceLon / 2) * sin(differenceLon / 2);

    float c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;  // Distance in meters


}

float calculateBearing(coordinates_t loc1, coordinates_t loc2) { //formula to calculate bearing/compass direction of the second coordinate from the first in degrees and sort it into actual directions
    float lat1 = degToRad(loc1.lat);
    float lon1 = degToRad(loc1.lon);
    float lat2 = degToRad(loc2.lat);
    float lon2 = degToRad(loc2.lon);

    float differenceLon = lon2 - lon1;

    float y = sin(differenceLon) * cos(lat2);
    float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(differenceLon);

    float bearing = atan2(y, x);
    bearing = radToDeg(bearing); // Convert to degrees

    if (bearing < 0) {
        bearing += 360; // Normalize to 0-360 degrees
    }
    return bearing;


}


float degToRad(float degrees) { //helper function for switching to radians
    return degrees * (PI / 180.0);
}
float radToDeg(float radians) { //helper function for switching to degrees
    return radians * (180.0 / PI);
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

