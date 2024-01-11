#include "LPC17xx.h"
#include "usart0.h"
#include "epoch.h"
#include <stdio.h>

void RTC_IRQHandler(void);
void RTC_Configuration(void);
void readDataTime();

extern uint8_t date[8];
extern uint8_t time[4];


void enableRTC(void){
	NVIC_EnableIRQ(RTC_IRQn);
}

void disableRTC(void){	
	NVIC_DisableIRQ(RTC_IRQn);
}


void RTC_Configuration(void){
	LPC_RTC->CCR = 1;
	LPC_RTC->ILR = 1;
	LPC_RTC->CIIR = 1;
		
	NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(void){
	char * n1 = "Tik ";
	char * n2 = "Tak ";
	char * napis;
	static char flag = 0;
	
	if(flag == 0){
		flag = 1;
		napis = n1;
	}
	else if(flag == 1){
		flag = 0;
		napis = n2;
	}
	
	//tempPrint(napis);
	readDataTime();	
	
	LPC_RTC->ILR = 1;
}

void readDataTime(){
	LPC_RTC->CCR = 0;
	uint8_t sec = LPC_RTC->SEC;
	uint8_t min = LPC_RTC->MIN;
	uint8_t hour = LPC_RTC->HOUR;
	
	uint8_t dom = LPC_RTC->DOM;
	uint8_t month = LPC_RTC->MONTH;
	uint16_t year = LPC_RTC->YEAR;
	
	

	tempPrintInt(hour); tempPrint(" "); tempPrintInt(min); tempPrint(" "); tempPrintInt(sec); tempPrint("\r\n");
	
	time[0] = hour / 10;
	time[1] = hour % 10;
	
	time[2] = min / 10;
	time[3] = min % 10;
	
	date[0] = dom / 10;
	date[1] = dom % 10;
	
	date[2] = month / 10;
	date[3] = month % 10;
	
	date[7] = year % 10;
	year /= 10;
	date[6] = year % 10;
	year /= 10;
	date[5] = year % 10;
	year /= 10;
	date[4] = year % 10;
	year /= 10;
	
	
	LPC_RTC->CCR = 1;
}

void setDateTime(){
	LPC_RTC->CCR = 0;
	uint8_t hour = time[0]*10 + time[1];
	uint8_t min = time[2]*10 + time[3];

	uint8_t dom = date[0]*10 + date[1];	
	uint8_t month = date[2]*10 + date[3];
	uint16_t year = date[4]*1000 + date[5]*100 + date[6]*10 + date[7];
	tempPrintInt(min); tempPrint(" "); tempPrintInt(hour); tempPrint("\r\n");
		
	LPC_RTC->SEC = 0;
	LPC_RTC->MIN = min;
	LPC_RTC->HOUR = hour;

	LPC_RTC->DOM = dom;
	LPC_RTC->MONTH = month;
	LPC_RTC->YEAR = year;
	
	LPC_RTC->CCR = 1;
}

