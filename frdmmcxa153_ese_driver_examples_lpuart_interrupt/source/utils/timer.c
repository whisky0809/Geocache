/*
 * timer.c
 *
 *  Created on: 14 Jun 2025
 *      Author: henri
 */
#include <MCXA153.h>
#include "gps/gps.h"
#include "sdCard/sdCard.h"
#include "timer.h"
int won = 0;
int ms = 0;

void timerInit()
{
	SysTick_Config(48000);
	NVIC_SetPriority (SysTick_IRQn, 7);
}
int* getMs()
{
	return &ms;
}
int* getWon()
{
	return &won;
}
void setWon(int val)
{
won = val;
}
void SysTick_Handler(void)
{
	 	ms++;

	    if((ms % 1000) == 0 && won == 0 && getFix() == 0) // change fix condition to 1
	    {
	    	sdLog(29, getPosition());
	    }
	    if((ms % 5000) == 0 && won == 0)
	    {
	    	//won = 1;
	    }

}
