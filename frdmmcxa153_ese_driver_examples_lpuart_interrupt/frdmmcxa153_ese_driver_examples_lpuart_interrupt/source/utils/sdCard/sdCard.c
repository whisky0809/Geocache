/*
 * sdCard.c
 *
 *  Created on: 18 May 2025
 *      Author: henri
 */
#include "../source/utils/sdCard/FatFs/ff.h"		/* Declarations of FatFs API */
#include "../source/utils/sdCard/sdCard.h"		/* Declarations of FatFs API */
#include "../source/utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include "../source/utils/gps/gps.h"

#include <stdio.h>

int u = 1;
FATFS FatFs;    // File system object
FIL Fil;        // File object
UINT bw, br;    // bw = bytes written, br = bytes read
FRESULT fr;
char readBuf[512];  // Buffer to read data into


void sdInit()
{
	fr = f_mount(&FatFs, "", 0);		// Give a work area to the default drive
	    	    	f_unlink(PATHNAME);
	    	    	if (fr != FR_OK) {
	    	    		    // Handle mount failure
	    	    		    printf("Mount failed with error code %d\n", fr);
	    	    		    while (1);
	    	    		}
}
void sdWrite()
{
	fr = f_open(&Fil, FILENAME, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);	//Create a file
	    	    	if (fr == FR_OK) {
	    	    		fr = f_lseek(&Fil, f_size(&Fil));
	        	    	if (fr == FR_OK) {
	        	    		//f_write(&Fil, "Henri,Luka,Beer\r\n",24  , &bw);	// Write data to the fil
	        	    		//f_printf(&Fil, " %d \n\r", u);
	    	    		}
	    	    		fr = f_close(&Fil);
	    	    	}
}
void sdLog(float temp, coordinates_t pos)
{
	fr = f_open(&Fil, FILENAME, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);	//Create a file
	    	    	if (fr == FR_OK) {
	    	    		fr = f_lseek(&Fil, f_size(&Fil));
	        	    	if (fr == FR_OK) {
	        	    		f_printf(&Fil, " %lf,%c,%lf,%c,%lf \n\r", pos.lat,pos.latDir,pos.lon, pos.lonDir, temp);
	    	    		}
	    	    		fr = f_close(&Fil);
	    	    	}
}
void sdRead(){
	fr = f_open(&Fil, FILENAME, FA_READ);
	int amount = f_size(&Fil) / sizeof(readBuf);
	amount++;
	printf("<"); //delimiter for laptop application
	printf("%d\n", amount);

	    	    	if (fr == FR_OK) {
	    	    		for(int u = 0; u < amount; u++)
	    	    		{
	    	    			do
	    	    			{
	    	    				f_read(&Fil,readBuf,sizeof(readBuf) - 1, &br );
	    	    				for (int i = 0; i<br; i++)
	    	    				{
	    	    					//while(lpuart0_getChar() != '<')    handshake with laptop application to ensure data
	    	    					//was stored properly before sending the next chunk of 512
	    	    					printf("%c", readBuf[i]);
	    	    				}

	    	    			}while (br == sizeof(readBuf));
	    	    		}
	printf(">"); //delimiter for laptop application
    printf("\n\n\r");
    f_close(&Fil);
}

}
