#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "string.h"
#include "GPIO_LPC17xx.h"
#include "image.h"

#define OLED_RST_1 GPIO_PinWrite(2,0,1);
#define OLED_RST_0 GPIO_PinWrite(2,0,0);

#define OLED_DC_0 GPIO_PinWrite(4,29,0);
#define OLED_DC_1 GPIO_PinWrite(4,29,1);

#define OLED_CS_0 GPIO_PinWrite(0,16,0);
#define OLED_CS_1 GPIO_PinWrite(0,16,1);

void sendSSP(uint8_t data);

void RTC_IRQHandler(void);

void SSP_Configuration(void);

void USART_Configuration(void);

void tempPrint(const char * napis);

void RTC_Configuration(void);

volatile uint32_t msTicks = 0;

void delay(int t) //ms
{
	msTicks = 0;
	while(t> msTicks);
}
void SysTick_Handler(void)
{
	msTicks++;
}
void OLED_reset(){
	OLED_RST_1
	delay(100);
	OLED_RST_0
	delay(100);
	OLED_RST_1
	delay(100);
}
void OLED_writeReg(uint8_t data){
		OLED_DC_0
		OLED_CS_0
		sendSSP(data);
		OLED_CS_1
}
void OLED_writeData(uint8_t data){
		OLED_DC_1
		OLED_CS_0
		sendSSP(data);
		OLED_CS_1
	
}
void OLED_initRegisters(){
    OLED_writeReg(0xAE);//--turn off oled panel

    OLED_writeReg(0x02);//---set low column address
    OLED_writeReg(0x10);//---set high column address

    OLED_writeReg(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_writeReg(0x81);//--set contrast control register
    OLED_writeReg(0xA0);//--Set SEG/Column Mapping a0/a1
    OLED_writeReg(0xC0);//Set COM/Row Scan Direction
    OLED_writeReg(0xA6);//--set normal display a6/a7
    OLED_writeReg(0xA8);//--set multiplex ratio(1 to 64)
    OLED_writeReg(0x3F);//--1/64 duty
    OLED_writeReg(0xD3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    OLED_writeReg(0x00);//-not offset
    OLED_writeReg(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_writeReg(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_writeReg(0xD9);//--set pre-charge period
    OLED_writeReg(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_writeReg(0xDA);//--set com pins hardware configuration
    OLED_writeReg(0x12);
    OLED_writeReg(0xDB);//--set vcomh
    OLED_writeReg(0x40);//Set VCOM Deselect Level
    OLED_writeReg(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_writeReg(0x02);//
    OLED_writeReg(0xA4);// Disable Entire Display On (0xa4/0xa5)
    OLED_writeReg(0xA6);// Disable Inverse Display On (0xa6/a7)

}

void OLED_init(){
		OLED_reset();
	
		OLED_initRegisters();
		delay(200);
	
		OLED_writeReg(0xAF);
	
}

void OLED_display(const uint8_t * image){
	uint16_t page, column, temp;
	for(page =0; page <8; page++)
	{
		OLED_writeReg(0xB0 + page);
		OLED_writeReg(0x02);
		OLED_writeReg(0x10);
		for(column = 0; column < 128; column++){
			temp = image[(7-page)+column*8];
			OLED_writeData(temp);
		}
	}
		

}
void testPin016(){
	LPC_GPIO0->FIODIR |= (1<<16);
	LPC_GPIO0->FIOSET |= (1<<16); // Output HIGH
	delay (100);
	LPC_GPIO0->FIOCLR |= (1<<16); // Output LOW
	delay (100);
	
}


int main(){
	
	//SysTick
	SysTick_Config((SystemCoreClock / 1000) - 1); // 1000 tick/sec - 1ms
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	
	//SSP
	SSP_Configuration();
	
	//OLED init
	OLED_init();
	delay(200);
	OLED_display(gImage_1in3);
	

	while(1){
		//OLED_CS_0
		//OLED_CS_1
		//delay(1000)s
		sendSSP(12);
	}

	return 0;	
}
void SSP_Configuration(void){
	LPC_SC->PCONP |= (1<<21); // timer dla ssp

	PIN_Configure(0,15,2,0,0); //SCK 3rd function is for SSP, 4th SPI
	PIN_Configure(0,16,0,2,0); //CS
	GPIO_SetDir(0,16,GPIO_DIR_OUTPUT);
	PIN_Configure(0,17,2,0,0); //MISO0
	PIN_Configure(0,18,2,0,0); //MOSI0
	
	PIN_Configure(2,0,0,0,0); //P2.0 RES
	PIN_Configure(4,29,0,0,0); //P2.0 RES
	
	//SSP config
	LPC_SC->PCONP |= (1<<21); // power control
	LPC_SSP0->CR0 = 1 | (1 << 1) | (1 << 2); // 8bit frame
	
	//SPI CPOL = 1 CPHA = 1 format
	LPC_SSP0->CR0 |= (1 << 6); // clock high between frames
	LPC_SSP0->CR0 |= (1 << 7); // CPHA
		
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
	//OLED_CS_0
	while((LPC_SSP0->SR & (1<<1)) == 1); // waits for not empty
	LPC_SSP0->DR = data; 
	
	while((LPC_SSP0->SR & (1<<4)) == 1); // not busy
	while((LPC_SSP0->SR & (1<<0)) == 1); // empty
	//OLED_CS_1
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

