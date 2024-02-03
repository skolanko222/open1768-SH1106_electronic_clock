#include "LPC17xx.h"

#ifndef SYSTICK_INCLUDE
	#define SYSTICK_INCLUDE

	volatile uint32_t msTicks = 0;
	volatile uint32_t pulseTicks =0;

	void delay(int t);

	void SysTick_Handler(void);


	void delay(int t) //ms
	{
		msTicks = 0;
		while(t> msTicks);
	}
	void SysTick_Handler(void)
	{
		msTicks++;
		pulseTicks++;
	}
	int SysTick_Get(){
		return pulseTicks;
	}
#else
	
#endif