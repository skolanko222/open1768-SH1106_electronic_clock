#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Driver_SPI.h"
//#include "image.h"
#include "spi_config.h"
#include "rtc_timer.h"
#include "OLED_driver.h"
#include "SysTick.h"
unsigned char obrazek[1030];
int main(){
	//unsigned char obrazek[1030];
	for(int i = 0; i < 1030; i++){
		obrazek[i]=0X00;
	}

	//SysTick
	SysTick_Config((SystemCoreClock / 1000) - 1); // 1000 tick/sec - 1ms
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	
	PIN_Configure(2,0,0,0,0); //P2.0 RES
	PIN_Configure(4,29,0,0,0); //P2.0 RES
	GPIO_SetDir(2,0,GPIO_DIR_OUTPUT);
	GPIO_SetDir(4,29,GPIO_DIR_OUTPUT);
	
	//SPI Lib
	SPI_Configuration();

	OLED_init();
	delay(200);
	
	obrazek[0] = 0xFF;
	
	OLED_display(obrazek); 
	while(1){
		
		//OLED_display(obrazek); 
		
	}

	return 0;	
}

