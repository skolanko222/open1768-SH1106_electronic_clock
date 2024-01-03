#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Driver_SPI.h"
#include "image.h"
#include "spi_config.h"
#include "rtc_timer.h"
#include "OLED_driver.h"
#include "SysTick.h"
#include "Board_Joystick.h"

unsigned char obrazek[1030];

unsigned char (*smallDigits[10])[16] = {smallZero, smallOne, smallTwo, smallThree, smallFour, smallFive, smallSix, smallSeven, smallEight, smallNine};
unsigned char (*bigDigits[10])[32] = {bigZero, bigOne, bigTwo, bigThree, bigFour, bigFive, bigSix, bigSeven, bigEight, bigNine};

unsigned char (*arrayOfTables[8])[16] = {smallZero,smallZero,smallZero,smallZero,smallZero,smallZero,smallZero,smallZero};
unsigned char (*arrayOfTables2[8])[32] = {bigZero,bigZero,bigZero,bigZero};

uint64_t epoch;


int main(){
	//unsigned char obrazek[1030];
	Joystick_Initialize();

	//SysTick
	SysTick_Config((SystemCoreClock / 1000) - 1); // 1000 tick/sec - 1ms
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	
	//SPI Lib
	SPI_Configuration();

	OLED_init();
	delay(200);
	
	drawDate(obrazek,arrayOfTables);
	drawTime(obrazek, arrayOfTables2);
	
	int configMode = 0; // 0: normal mode, 1: config mode
  int currentDigit = 0; // 0-11, liczba aktualnie edytowana
  int pulseState = 0; // 0: normal, 1: pulsing
  unsigned int pulseTime = 0; // Czas ostatniej zmiany stanu pulsowania
  int whichNext = 0; // zmiana na nastepny element
  unsigned char (*originalValue)[16] = arrayOfTables[currentDigit]; // wartosc dla migajacego elementu (zapisana wczesniej)
	
	uint8_t date[8] = {3,1,1,2,2,0,2,3};
	uint8_t time[4] = {2,1,3,7};
	char flag = 0;

	OLED_display(obrazek);
	while(1){
        if(configMode){
            // W config mode
						if(flag)
							disableRTC
						flag = 0;
					
            if(Joystick_GetState() == JOYSTICK_CENTER){
                // Wyjdz z config mode
                configMode = 0;
								flag = 1;
            } else if(Joystick_GetState() == JOYSTICK_RIGHT){
                // Przejdz do nastepnego elementu
                currentDigit = (currentDigit + 1) % 12; // Assuming 12 digits
            } else if(Joystick_GetState() == JOYSTICK_LEFT){
                // Przejdz do poprzedniego elementu
                currentDigit = (currentDigit - 1) % 12; // Assuming 12 digits
                if (currentDigit < 0) currentDigit = 11;
            } else if(Joystick_GetState() == JOYSTICK_UP){
                // Zwieksz wartosc aktualnego elementu
                if (currentDigit < 8){
                    if(currentDigit == 0){
                        date[currentDigit] = (date[currentDigit] + 1) % 4;
                        arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                    }
                    else if(currentDigit == 2){
                        date[currentDigit] = (date[currentDigit] + 1) % 3;
                        arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                    }
                    else {
                        date[currentDigit] = (date[currentDigit] + 1) % 10;
                        arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                    }
                }
                else {
                    if(currentDigit == 8){
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 3;
                        arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                    }
                    else if(currentDigit == 10){
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 6;
                        arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                    }
                    else {
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 10;
                        arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                    }
                }
            } else if(Joystick_GetState() == JOYSTICK_DOWN){
                // Zmniejsz wartosc aktualnego elementu
                if (currentDigit < 8){
                    if(currentDigit == 0)
                        date[currentDigit] = (date[currentDigit] - 1) % 4;
                    else if(currentDigit == 2)
                        date[currentDigit] = (date[currentDigit] - 1) % 3;
                    else 
                        date[currentDigit] = (date[currentDigit] - 1) % 10;
                    arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                }
                else {
                    if(currentDigit == 8)
                        time[currentDigit-8] = (time[currentDigit-8] - 1) % 3;
                    else if(currentDigit == 10)
                        time[currentDigit-8] = (time[currentDigit-8] - 1) % 6;
                    else 
                        time[currentDigit-8] = (time[currentDigit-8] - 1) % 10;
                    arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                }
            }

            if (currentDigit < 8){
                // Zapisz wartosc dla migajacego elementu
                originalValue = arrayOfTables[currentDigit];
            }
            else {
                originalValue = arrayOfTables2[currentDigit-8];
            }

            // Niech cyfra miga co 500ms
            if(SysTick_Get() - pulseTime > 500){ // Systick_Get() zwraca czas w ms
                pulseState = 1 - pulseState; // Wlacz/wylacz migotanie
                pulseTime = SysTick_Get();
                if(pulseState){
                    // Pokaz aktualny element
                    arrayOfTables[currentDigit] = originalValue;
                } else {
                    // Pokaz pusty element
                    arrayOfTables[currentDigit] = 0;
                }
            }
        } else {
            // W normalnym trybie
						if(flag)
							enableRTC
						flag = 0;
					
            if(Joystick_GetState() == JOYSTICK_CENTER){
                // Wejdz w config mode
                configMode = 1;
                currentDigit = 0;
								flag = 1;
                pulseTime = SysTick_Get();
            }
        }
				
        drawDate(obrazek,arrayOfTables);
        drawTime(obrazek, arrayOfTables2);
        OLED_display(obrazek); 
    }

	return 0;	
}

