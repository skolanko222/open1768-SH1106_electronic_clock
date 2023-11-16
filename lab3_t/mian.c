#include "LPC17xx.h"
#include "PIN_LPC17xx.h"

#include "Driver_USART.h"
// #include "Driver_USART.h"

extern ARM_DRIVER_USART Driver_USART0;
volatile uint32_t msTicks = 0;

void delay(int t)
{
	msTicks = 0;
	while(t> msTicks);
}

int main(void){
	SysTick_Config(SystemCoreClock / 100); //10ms
	//LPC_SC->PCONP |= 1<<24;
	
	PIN_Configure(0,2,1,2,0);
	PIN_Configure(0,3,1,2,0);
	
	LPC_UART0->LCR = 3 | (1<<7); // 8 bit, konfiguracja
	LPC_UART0->DLL = 27;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR = 3;
	
	const char napis[] = "napisTest";
	while(1)
	{
						
			for(int i = 0; i < sizeof(napis) ; i++){
				
				if(LPC_UART0->LSR >> 6 == 1){
				LPC_UART0->THR = napis[i];
				}
				else{
					i--;
				}
			}
			delay(100);
		}
	return 0;
}
