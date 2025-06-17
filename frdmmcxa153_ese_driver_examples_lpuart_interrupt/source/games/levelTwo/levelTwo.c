/*
 * levelTwo.c
 *
 *  Created on: 11 Jun 2025
 *      Author: henri
 */
#include "../../utils/colourSensor/color_classifier.h"
#include "../gpsGame/gpsGame.h"
#include "levelTwo.h"
#include "../../utils/flag.h"
#include "../../utils/timer.h"
#include <stdbool.h>
int color;
int currentMs;
int prevMs = 0;
int interval = 5000;
bool levelTwo()
{
if(getColor() != color)
{
color  = getColor();
switch(color)
{
case BLUE:
//flag for an update LCD function or immediate print
setLCDFlag(1);
break;
case RED:
//flag for an update LCD function or immediate print
setLCDFlag(2);


break;
case GREEN:
//flag for an update LCD function or immediate print
setLCDFlag(3);

break;
}
}
else{

//add time logic here
currentMs = *getMs();
if((currentMs - prevMs) >= interval)
{
prevMs = currentMs;
setLCDFlag(0);
}
}
if(*getDistance() < 10){return true;}
else{return false;}
}

