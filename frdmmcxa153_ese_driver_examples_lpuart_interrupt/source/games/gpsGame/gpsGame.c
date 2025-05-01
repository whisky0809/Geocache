/*
 * gpsGame.c
 *
 *  Created on: 28 Apr 2025
 *      Author: henri
 */


#include "gpsGame.h"
#include "../source/utils/gps/gps.h"
#include "math.h"
#include "stdlib.h"
/*
float distance(coordinates_t loc1, coordinates_t loc2) { //Haversine distance formula


	float differenceLat = degToRad(loc2.lat - loc1.lat);
    float differenceLon = degToRad(loc2.lon - loc1.lon);



    float a = sin(differenceLat / 2) * sin(differenceLat / 2) +
               cos(degToRad(loc1.lat)) * cos(degToRad(loc2.lat)) *
               sin(differenceLon / 2) * sin(differenceLon / 2);

    float c = 2 * atan2(sqrt(a), sqrt(1 - a));

    float d = EARTH_RADIUS * c;  // Distance in meters
    if(d < TARGET_RADIUS)
    {
    	//arrived at location

    }
    return d;

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
*/
