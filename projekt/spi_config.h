#include "Driver_SPI.h"
#ifndef SPI_INCLUDE
	#define SPI_INCLUDE

extern ARM_DRIVER_SPI Driver_SPI2;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI2;

#define OLED_RST_1 GPIO_PinWrite(2,0,1);
#define OLED_RST_0 GPIO_PinWrite(2,0,0);

#define OLED_DC_0 GPIO_PinWrite(4,29,0);
#define OLED_DC_1 GPIO_PinWrite(4,29,1);

void SPI_Configuration(){
	SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}

void SPI_Send(const void *data, uint32_t num){
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
		SPIdrv->Send(data,num);
		ARM_SPI_STATUS state = SPIdrv->GetStatus();
		do{
			state = SPIdrv->GetStatus();
		}while(state.busy==1);
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}
#else
	
#endif