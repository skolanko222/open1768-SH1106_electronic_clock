#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Driver_SPI.h"
#include "image.h"
#include "spi_config.h"
#include "rtc_timer.h"
#include "OLED_driver.h"
#include "SysTick.h"
#include "Board_Joystick.h"
#include <stdio.h>

unsigned char obrazek[1030];

unsigned char (*smallDigits[10])[16] = {smallZero, smallOne, smallTwo, smallThree, smallFour, smallFive, smallSix, smallSeven, smallEight, smallNine};
unsigned char (*bigDigits[10])[32] = {bigZero, bigOne, bigTwo, bigThree, bigFour, bigFive, bigSix, bigSeven, bigEight, bigNine};

unsigned char (*arrayOfTables[8])[16] = {smallZero,smallZero,smallZero,smallZero,smallZero,smallZero,smallZero,smallZero};
unsigned char (*arrayOfTables2[4])[32] = {bigZero,bigZero,bigZero,bigZero};

int8_t date[8] = {2,7,1,1,2,0,2,3};
int8_t time[4] = {2,1,5,9};

//uint64_t epoch;


int main(){
	//unsigned char obrazek[1030];
	Joystick_Initialize();

	//SysTick
	SysTick_Config((SystemCoreClock / 1000) - 1); // 1000 tick/sec - 1ms
	
	//usart
	USART_Configuration();
	
	//RTC
	RTC_Configuration();
	readDataTime();
	if(date[4]>2 && (date[5]>0 || date[6]>2)){
		date[0] = 3;
		date[1] = 1;
		date[2] = 1;
		date[3] = 2;
		date[4] = 2;
		date[5] = 0;
		date[6] = 2;
		date[7] = 4;
		time[0] = 2;
		time[1] = 3;
		time[2] = 5;
		time[3] = 9;
		setDateTime();
	}

	
	//SPI Lib
	SPI_Configuration();

	OLED_init();
	delay(200);
	
	drawDate(obrazek, arrayOfTables);
	drawTime(obrazek, arrayOfTables2, 0);
	
	int configMode = 0; // 0: normal mode, 1: config mode
  int currentDigit = 0; // 0-11, cyfra aktualnie edytowana
  int pulseState = 0; // 0: normal, 1: pulsing
  unsigned int pulseTime = 0; // Czas ostatniej zmiany stanu pulsowania
  unsigned int originalValueSmall = 0;
	unsigned int originalValueBig = 0;
	unsigned char flagColon = 0;
	
		
	char flag = 0;

	OLED_display(obrazek);
	while(1){
			uint32_t state = Joystick_GetState();
			delay(165);
        if(configMode){
            // W config mode
					
						if(flag){
							disableRTC();							
							flagColon = 1;
						}
							
						flag = 0;
					
            if(state == JOYSTICK_CENTER){
                // Wyjdz z config mode
                configMode = 0;
								flag = 1;
            } else if(state == JOYSTICK_RIGHT){
                // Przejdz do nastepnego elementu
								if(pulseState==0){
									if (currentDigit<8){
											arrayOfTables[currentDigit] = smallDigits[originalValueSmall];
									}
                  else {
											arrayOfTables2[currentDigit-8] = bigDigits[originalValueBig];
									}
								}
                currentDigit = (currentDigit + 1) % 12; // Assuming 12 digits
            } else if(state == JOYSTICK_LEFT){
							if(pulseState==0){
									if (currentDigit<8){
											arrayOfTables[currentDigit] = smallDigits[originalValueSmall];
									}
                  else {
											arrayOfTables2[currentDigit-8] = bigDigits[originalValueBig];
									}
								}
                // Przejdz do poprzedniego elementu
                currentDigit = (currentDigit - 1) % 12; // Assuming 12 digits
                if (currentDigit < 0) currentDigit = 11;
            } else if(state == JOYSTICK_UP){
                // Zwieksz wartosc aktualnego elementu
                if (currentDigit < 8){ // data
                    if(currentDigit == 0){
                        date[currentDigit] = (date[currentDigit] + 1) % 4;
                    }
                    else if(currentDigit == 2 || (currentDigit == 1 && date[0] == 3)){
                        date[currentDigit] = (date[currentDigit] + 1) % 2;
                    }
										else if(currentDigit == 3 && date[2] == 1){
                        date[currentDigit] = (date[currentDigit] + 1) % 3;
                    }
                    else {
                        date[currentDigit] = (date[currentDigit] + 1) % 10;
                    }
									  arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                }
                else { // godzina
                    if(currentDigit == 8){
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 3;
                    }
                    else if(currentDigit == 10){
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 6;
                    }
										else if(currentDigit == 9 && time[0]== 2)
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 4;
                    else {
                        time[currentDigit-8] = (time[currentDigit-8] + 1) % 10;
                    }
										arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                }
            } else if(state == JOYSTICK_DOWN){
                // Zmniejsz wartosc aktualnego elementu
                if (currentDigit < 8){
                    if(currentDigit == 0){
											date[currentDigit] = ((date[currentDigit] - 1) % 4) < 0 ? 4+((date[currentDigit] - 1) % 4) : ((date[currentDigit] - 1) % 4) ;
										}
											else if(currentDigit == 2 || (currentDigit == 1 && date[0] == 3)){
                        date[currentDigit] = ((date[currentDigit] - 1) % 2) < 0 ? 2+((date[currentDigit] - 1) % 2) : ((date[currentDigit] - 1) % 2) ;
										}
										else if(currentDigit == 3 && date[2] == 1){
                        date[currentDigit] = ((date[currentDigit] - 1) % 3) < 0 ? 3+((date[currentDigit] - 1) % 3) : ((date[currentDigit] - 1) % 3) ;
                    }
                    else 
                        date[currentDigit] = ((date[currentDigit] - 1) % 10) < 0 ? 10+((date[currentDigit] - 1) % 10) : ((date[currentDigit] - 1) % 10);
                    arrayOfTables[currentDigit] = smallDigits[date[currentDigit]];
                }
                else {
                    if(currentDigit == 8)
												time[currentDigit-8] = ((time[currentDigit-8] - 1) % 3) < 0 ? 3+((time[currentDigit-8] - 1) % 3) : ((time[currentDigit-8] - 1) % 3) ;
										else if(currentDigit == 9 && time[0]== 2)
                        time[currentDigit-8] = ((time[currentDigit-8] - 1) % 4) < 0 ? 4+((time[currentDigit-8] - 1) % 4) : ((time[currentDigit-8] - 1) % 4) ;
                    else if(currentDigit == 10)
                         time[currentDigit-8] = ((time[currentDigit-8] - 1) % 6) < 0 ? 6+((time[currentDigit-8] - 1) % 6) : ((time[currentDigit-8] - 1) % 6) ;
                    else 
                        time[currentDigit-8] = ((time[currentDigit-8] - 1) % 10) < 0 ? 10+((time[currentDigit-8] - 1) % 10) : ((time[currentDigit-8] - 1) % 10) ;
                    arrayOfTables2[currentDigit-8] = bigDigits[time[currentDigit-8]];
                }
            }

            if (currentDigit < 8){
                // Zapisz wartosc dla migajacego elementu
                originalValueSmall = date[currentDigit];
            }
            else {
                originalValueBig = time[currentDigit-8];
            }
						
            // Niech cyfra miga co 500ms
            if(pulseTicks > 250){ // Systick_Get() zwraca czas w ms
								pulseTicks = 0;
                pulseState = 1 - pulseState; // Wlacz/wylacz migotanie
                if(pulseState){
                    // Pokaz aktualny element
                    if (currentDigit<8){
											arrayOfTables[currentDigit] = smallDigits[originalValueSmall];
										}
                    else {
											arrayOfTables2[currentDigit-8] = bigDigits[originalValueBig];
										}
                } else {
                    // Pokaz pusty element
										if (currentDigit<8){
											arrayOfTables[currentDigit] = smallBox;
										}
                    else {
											arrayOfTables2[currentDigit-8] = bigBox;
										}
                }
            }
						
        } else {
            // W normalnym trybie
						if(flag){
							//epoch = calc_epoch_from_date(date,time);
							setDateTime();
							enableRTC();
						}
						flag = 0;
						//calc_date_form_epoch(epoch,date,time);
					
            if(state == JOYSTICK_CENTER){
                // Wejdz w config mode
                configMode = 1;
                currentDigit = 0;
								flag = 1;
                pulseTime = SysTick_Get();
            }
						for(int i = 0; i < 12;i++){
							if(i<8){
								arrayOfTables[i]= smallDigits[date[i]];
							}
							else if(i>7){
								arrayOfTables2[i-8]= bigDigits[time[i-8]];
							}
					}
						if(pulseTicks > 420){ 
								pulseTicks = 0;
                flagColon = 1 - flagColon; // Wlacz/wylacz migotanie

            }
        }
				
        drawDate(obrazek,arrayOfTables);
        drawTime(obrazek, arrayOfTables2, flagColon);
        OLED_display(obrazek); 
    }

	return 0;	
}

