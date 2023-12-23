#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Driver_SPI.h"
#include "image.h"
#include "spi_config.h"
#include "rtc_timer.h"
#include "OLED_driver.h"
#include "SysTick.h"
#include "Board_Joystick.h"
unsigned char obrazek[1030];
int main(){
	//unsigned char obrazek[1030];
	Joystick_Initialize();

	//SysTick
	SysTick_Config((SystemCoreClock / 1000) - 1); // 1000 tick/sec - 1ms
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	
	//SPI Lib
	SPI_Configuration();

	OLED_init();
	delay(200);

	unsigned char (*arrayOfTables[8])[16] = {smallOne, smallTwo, smallThree, smallFour, smallFive,smallSix, smallSeven, smallEight};
	drawDate(obrazek,arrayOfTables);
	unsigned char (*arrayOfTables2[8])[32] = {bigOne, bigTwo, bigThree, bigFour};
	drawTime(obrazek, arrayOfTables2);
	


	OLED_display(obrazek);
	while(1){
		drawTime(obrazek, arrayOfTables2);
		if(Joystick_GetState() == JOYSTICK_CENTER){
			// pierwsza cyfra daty miga
			// gora-dol zmienia wartosc 
			// lewo-prawo zmienia liczbe do zmiany
			// klikniecie srodka wychodzi z konfiguracji
			// jesli index przekracza rozmiar tablicy to reset do pierwszego elementu
			for(int i=0;i<1030;i++){
				obrazek[i]=0;
			}
		}
		OLED_display(obrazek); 
		
	}

	return 0;	
}

