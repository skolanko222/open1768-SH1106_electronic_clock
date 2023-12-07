#include "usart.h"

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
	
	LPC_RTC->ILR = 1;

}