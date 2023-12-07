#include "usart.h"

void sendSSP(uint16_t data){
	while(1){
		//if((LPC_SSP0->SR & 1) == 0){
			LPC_SSP0->DR = data;
			tempPrint("sendSSP");
			break;
			
			
		//}
	}
	
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
	sendSSP(100);
	LPC_RTC->ILR = 1;

}
int main(){
	
	//usart
	//usartConfigure();
	
	//RTC
	LPC_RTC->CCR = 1;
	LPC_RTC->ILR = 1;
	LPC_RTC->CIIR = 1;
	
	PIN_Configure(0,2,1,2,0); // funkcja 1, mode 2 (brak pull up/down)
		PIN_Configure(0,3,1,2,0);
		//usart
		LPC_UART0->LCR = 3 | (1<<7);
		LPC_UART0->DLL = 27; // dzielnik peryferyjny 2, 256 max
		LPC_UART0->DLM = 0; // tu naddatek x*256
		LPC_UART0->FCR = 6;
		LPC_UART0->LCR = 3;
	
	
	NVIC_EnableIRQ(RTC_IRQn);
	
	//SSP
	LPC_SC->PCONP |= (1<<21); // timer dla ssp

	PIN_Configure(0,15,2,0,0); //SCK
	PIN_Configure(0,16,2,0,0); //SSEL0
	PIN_Configure(0,17,2,0,0); //MISO0
	PIN_Configure(0,18,2,0,0); //MOSI0
	//LPC_PINCON->PINSEL0 |= (2 << 30); //SCK/0
	//LPC_PINCON->PINSEL1 |= 2 | (2<<2) | (2 << 4); //SSEL0 MISO0 MOSI0
	//LPC_PINCON->PINMODE0 |=


	//SSP config
	LPC_SSP0->CR1 = (1 << 1); // tryb master
	LPC_SSP0->CR0 = (1 << 1) | (1 << 2) | (1 << 3)| (1 << 4); // pierwsze 4 bity na
	LPC_SSP0->CR1 |= (1 << 1);
	
	//interupty SSP0IMSC
	while(1){
		
	
	}

	
	return 0;	
}
