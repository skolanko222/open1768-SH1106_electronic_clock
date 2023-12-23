#include "LPC17xx.h"
#include "SysTick.h"
#include "spi_config.h"
void OLED_reset();
	
void OLED_writeReg(uint8_t data);

void OLED_writeData(uint8_t data);

void OLED_initRegisters();

void OLED_init();

void OLED_display_pointer(unsigned char (* image)[1030]);
void OLED_display(unsigned char * image);

void OLED_reset(){
	OLED_RST_1
	delay(100);
	OLED_RST_0
	delay(100);
	OLED_RST_1
	delay(100);
}

void OLED_init(){
		OLED_reset();
		OLED_initRegisters();
		delay(200);
		OLED_writeReg(0xAF);
}

void OLED_display_pointer(unsigned char (* image)[1030]){ //v1
	uint16_t page, column, temp;
	for(page =0; page <8; page++)
	{
		OLED_writeReg(0xB0 + page); /*set page address*/ 
		OLED_writeReg(0x02);  /*set lower column address*/
		OLED_writeReg(0x10);  /*set higher column address*/
		for(column = 0; column < 128; column++){
			temp = (*image)[(7-page)+column*8];
			OLED_writeData(temp);
		}
	}
		

}
void OLED_display(unsigned char * image){//v2
	uint16_t page, column, temp;
	for(page =0; page <8; page++)
	{
		OLED_writeReg(0xB0 + page); /*set page address*/ 
		OLED_writeReg(0x02);  /*set lower column address*/
		OLED_writeReg(0x10);  /*set higher column address*/
		for(column = 0; column < 128; column++){
			temp = image[(7-page)+column*8];
			OLED_writeData(temp);
		}
	}
		

}
void OLED_writeReg(uint8_t data)
{
		OLED_DC_0
		SPI_Send((const void*)&data,sizeof(data));
}
void OLED_writeData(uint8_t data){
		OLED_DC_1
		SPI_Send((const void*)&data,sizeof(data));
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