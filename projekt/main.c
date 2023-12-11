#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "string.h"

void sendSSP(uint8_t data);

void RTC_IRQHandler(void);

void SSP_Configuration(void);

void USART_Configuration(void);

void tempPrint(const char * napis);

void RTC_Configuration(void);

int main(){
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	
	//SSP
	SSP_Configuration();
	
	while(1){
	}

	return 0;	
}
void SSP_Configuration(void){
	LPC_SC->PCONP |= (1<<21); // timer dla ssp

	PIN_Configure(0,15,2,0,0); //SCK 3rd function is for SSP, 4th SPI
	PIN_Configure(0,16,2,0,0); //SSEL0 - CS slave select
	PIN_Configure(0,17,2,0,0); //MISO0
	PIN_Configure(0,18,2,0,0); //MOSI0
	
	//SSP config
	LPC_SC->PCONP |= (1<<21); // power control
	LPC_SSP0->CR0 = 1 | (1 << 1) | (1 << 2); // 8bit frame
	
	//SPI CPOL = 1 CPHA = 1 format
	LPC_SSP0->CR0 |= (1 << 6); // clock high between frames
	LPC_SSP0->CR0 |= (1 << 7);
		
	LPC_SSP0->CR1 = (1 << 1); // The SSP controller will interact with other devices on the serial bus

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
	
	tempPrint(napis);
	//sendSSP(100);
	LPC_RTC->ILR = 1;

}

void sendSSP(uint8_t data){
	LPC_SSP0->DR = data; // za czy przed whilem?
	while((LPC_SSP0->SR & 1) == 0){ // waits for TFR FIFO to be empty
		tempPrint("sendSSP");
	}
}

void USART_Configuration(void){
	
	PIN_Configure(0,2,1,2,0); // funkcja 1, mode 2 (brak pull up/down)
		PIN_Configure(0,3,1,2,0);
		//usart
		LPC_UART0->LCR = 3 | (1<<7);
		LPC_UART0->DLL = 27; // dzielnik peryferyjny 2, 256 max
		LPC_UART0->DLM = 0; // tu naddatek x*256
		LPC_UART0->FCR = 6;
		LPC_UART0->LCR = 3;
}


void tempPrint(const char * napis)
{
		//strcat(napis,"\r");
		for(int i = 0; i < strlen(napis);i++){
				
				if(LPC_UART0->LSR >> 6 == 1){
				LPC_UART0->THR = napis[i];
				}
				else{
					i--;
				}
			}
}

