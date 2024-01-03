#include "LPC17xx.h"
#include "usart0.h"

void RTC_IRQHandler(void);
void RTC_Configuration(void);

extern uint64_t epoch;

#define disableRTC NVIC_DisableIRQ(RTC_IRQn);
#define enableRTC  NVIC_EnableIRQ(RTC_IRQn);


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
	
	tempPrint(napis);
	epoch++;
	LPC_RTC->ILR = 1;

}