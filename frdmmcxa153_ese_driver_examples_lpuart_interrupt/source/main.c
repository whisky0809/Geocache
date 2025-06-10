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
#include "../source/utils/sdCard/sdCard.h"
#include "../source/utils/temperatureSensor/lm35d_polling.h"

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
 	char abreviation[128];
}direction_t;

enum gameState
{
LEVEL1,
LEVEL2,
LEVEL3,
LEVEL4
};
coordinates_t targets[4];
// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------
int ms = 0;
int won = 0;
int wrote = 1;
// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------


int main(void)
{



	int u = 1;
    gpio_output_init();
    gpsInit();
    sdInit();
    lm35d_init();
    int gameState = LEVEL1; //change to settings from csv file
    SysTick_Config(48000);
    NVIC_SetPriority (SysTick_IRQn, 7);

           // Enable interrupts
  __enable_irq();

   //sdWrite();




   //use f_unlink to delete the file after the run

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

    	case LEVEL1:
    		//sdReadSettings(getTarget(),LEVEL1);
    		//add level One Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;
    	case LEVEL2:
    		//sdReadSettings(getTarget(),LEVEL2);

    		//add level Two Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;
    	case LEVEL3:
    		//sdReadSettings(getTarget(),LEVEL3);

    		//add level Three Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    		break;
    	case LEVEL4:
    		//sdReadSettings(getTarget(),LEVEL4);

    		//add level Four Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;

    	}

    	if(won == 1)
    	{
    		disableLpuart2();
    		int closed = 0;
    		while(!closed)
    		{
    			char c = lpuart0_getchar();
    				if(c == '<')
    				{
    					sdReadLogs(); //send logs to laptop
    					}

    				else if(c == '>')
    		    	{
    		    		sdSettings(); //read settings from laptop
    		   			}
    				else if(c == '*')
    				{
    				    closed = 1; //read settings from laptop
    				}
    		}
    		printf("done");
    		won = 2;
    	}




    }


}


// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------

void SysTick_Handler(void)
{
	 	ms++;

	    if((ms % 1000) == 0 && won == 0 && getFix() == 0)
	    {
	    	sdLog(12, getPosition());
	    }
	    if((ms % 5000) == 0 && won == 0)
	    {
	    	won = 1;
	    }

}

