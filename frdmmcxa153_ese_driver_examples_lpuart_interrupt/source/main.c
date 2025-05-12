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
#include "../source/games/gpsGame/gpsGame.h"
#include "../source/utils/comProtocols/GPIO/gpio_output.h"
#include "../source/utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "../source/utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include "../source/utils/sdCard/FatFs/ff.h"		/* Declarations of FatFs API */

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

typedef struct Direction
{
 	char abreviation[2];
}direction_t;

enum gameState
{
TUTORIAL,
LEVEL1,
LEVEL2,
LEVEL3,
LEVEL4
};
// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------


int main(void)
{




    gpio_output_init();
    gpsInit();
    int gameState = TUTORIAL;
    FATFS FatFs;    // File system object
    FIL Fil;        // File object
    UINT bw, br;    // bw = bytes written, br = bytes read
    FRESULT fr;

    	    	char readBuf[32];  // Buffer to read data into


    	    	fr = f_mount(&FatFs, "", 0);		// Give a work area to the default drive
    	    	if (fr != FR_OK) {
    	    		    // Handle mount failure
    	    		    printf("Mount failed with error code %d\n", fr);
    	    		    while (1);
    	    		}
    		    printf("Mount succesful with error code %d\n", fr);
    		    printf("disk_status: 0x%02X\n", disk_status(0));

    	    	fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);	//Create a file
    	    	if (fr == FR_OK) {
    	    		f_write(&Fil, "It works!\r\n", 11, &bw);	// Write data to the file
    	    		fr = f_close(&Fil);							// Close the file
    	    	}
    	    	else
    	    	{
    	    		 printf("Writing failed with error code %d\n", fr);
    	    			    while (1);
    	    	}

    	    	fr = f_open(&Fil, "newfile.txt", FA_READ);
    	    	if (fr == FR_OK) {
    	    	    f_read(&Fil, readBuf, sizeof(readBuf) - 1, &br);  // Read up to 31 bytes
    	    	    readBuf[br] = '\0';  // Null-terminate the string
    	    	    f_close(&Fil);
    	    	}
    	    	else
    	    	{
    	    		 printf("Reading failed with error code %d\n", fr);
    	    			    while (1);
    	    	}
    	    	 for(int i = 0; i < 31; i++)
    	    	 {
    	    	    printf("%c", readBuf[i]);
    	    	 }
    while(1)
    {
    	updatePosition(); // checks buffer for new relevant NMEA sentence every cycle
    	//might add seperate file to handle all the game controlling and switching stuff
    	if(getSuccessFlag())
    	{
    		gameState++;
    	}

    	switch(gameState)
    	{
    	case TUTORIAL:
    		//optional
    		break;
    	case LEVEL1:
    		//add level One Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;
    	case LEVEL2:
    		//add level Two Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;
    	case LEVEL3:
    		//add level Three Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    		break;
    	case LEVEL4:
    		//add level Four Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;

    	}
    }

/*

    	FATFS FatFs;    // File system object
    	FIL Fil;        // File object
    	UINT bw, br;    // bw = bytes written, br = bytes read
    	FRESULT fr;

    	char readBuf[32];  // Buffer to read data into


    	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive

    	fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file
    	if (fr == FR_OK) {
    		f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file
    		fr = f_close(&Fil);							/* Close the file
    	}

    	fr = f_open(&Fil, "newfile.txt", FA_READ);
    	if (fr == FR_OK) {
    	    f_read(&Fil, readBuf, sizeof(readBuf) - 1, &br);  // Read up to 31 bytes
    	    readBuf[br] = '\0';  // Null-terminate the string
    	    f_close(&Fil);
    	}
    	 for(int i = 0; i < 31; i++)
    	 {
    	    printf("%c", readBuf[i]);
    	 }*/
}


// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------


