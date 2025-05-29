/*
 * sdCard.h
 *
 *  Created on: 18 May 2025
 *      Author: henri
 */

#ifndef UTILS_SDCARD_SDCARD_H_
#define UTILS_SDCARD_SDCARD_H_
#include "../source/utils/gps/gps.h"


#define FILENAME "log.txt"
#define PATHNAME "0:/log.txt"
void sdWrite();
void sdLog(uint8_t temp, coordinates_t pos);
void sdRead();
void sdInit();


#endif /* UTILS_SDCARD_SDCARD_H_ */
