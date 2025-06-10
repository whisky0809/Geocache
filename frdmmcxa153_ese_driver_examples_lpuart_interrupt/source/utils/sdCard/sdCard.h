/*
 * sdCard.h
 *
 *  Created on: 18 May 2025
 *      Author: henri
 */

#ifndef UTILS_SDCARD_SDCARD_H_
#define UTILS_SDCARD_SDCARD_H_
#include "../source/utils/gps/gps.h"


#define LOGFILE "log.txt"
#define SETFILE "set.txt"

#define PATHNAME "0:/log.txt"
void sdWrite();
void sdRead(char* filename);
void sdBufWrite(char *filename, char *buffer, int size);
void sdLog(uint8_t temp, coordinates_t* pos);
void sdReadLogs();
void sdInit();


#endif /* UTILS_SDCARD_SDCARD_H_ */
