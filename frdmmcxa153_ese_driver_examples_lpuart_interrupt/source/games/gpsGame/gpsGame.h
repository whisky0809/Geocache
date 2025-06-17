/*
 * gpsGame.h
 *
 *  Created on: 28 Apr 2025
 *      Author: henri
 */

#ifndef GAMES_GPSGAME_GPSGAME_H_
#define GAMES_GPSGAME_GPSGAME_H_
#include "../source/utils/gps/gps.h"

typedef enum boolean
{
	FALSE = 0,
 	TRUE = 1
}bool_t;

#define EARTH_RADIUS 6371000  // Earth rad in meters
#define TARGET_RADIUS 10  // radius in m to target location for match

float distance(coordinates_t* loc1, coordinates_t* loc2);  //Haversine distance formula
float calculateBearing(coordinates_t* loc1, coordinates_t* loc2);
float degToRad(float degrees);
float radToDeg(float radians);
float* getDistance();
float* getBearing();


#endif /* GAMES_GPSGAME_GPSGAME_H_ */
