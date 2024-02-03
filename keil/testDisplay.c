#include "image.h"
#include <stdio.h>
#include <cstdint>
unsigned char obrazek[1030];

#define WIDTH 64
#define HEIGHT 128

//extern unsigned char gImage_1in3[1030];

unsigned char temp[16][2] = { {1,1}, {1,1}, {1,1} };

void consolDraw(unsigned char image[1030]){

	unsigned char (* pointer )[8] = (unsigned char (*)[8]) image;
	
	for(int i = 0; i < 128; i++){
		printf("i: %d\t",i);//std::cout << "i:" << i << "\t";
		for(int j = 0; j < 8; j++){
			for(int k = 0; k < 8; k++){
				printf("%c",((pointer[i][j]<<k) & 0b10000000)? '1' : ' ' );			
			}
		}
		printf("\n");
		
	}
}
void drawPixel(unsigned char image[1030], int x, int y, char value) { // value = 0 or 1
	if(x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) return;
	unsigned char byte = ((unsigned char (*)[8])image)[x][y/8];
	if(value) {
		byte |= 1 << (7-(y%8));
	} else {
		byte &= ~(1 << (7-(y%8)));
	}
	((unsigned char (*)[8])image)[x][y/8] = byte;

	return;
}

//array - zwykła tablica 2D
void drawArray(unsigned char image[1030], int x, int y, int width, int height, unsigned char array[width][height]) {
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			drawPixel(image, x+i, y+j, array[i][j]);
		}
	}
	return;
}
void drawDate(unsigned char * obrazek, unsigned char (*arrayOfigits[8])[16]){
	int startYSmall = 92;
	int digitWidthSmall = 8;
	int liczSmall = 0;
	int spaceSmall=0;
	
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[0]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[1]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall) - spaceSmall, 0, 8, 16, smallDot);
	spaceSmall+=4;
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[2]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[3]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall) - spaceSmall, 0, 8, 16, smallDot);
	spaceSmall+=4;
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[4]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[5]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[6]);
	drawArray(obrazek, startYSmall - digitWidthSmall*(liczSmall++) - spaceSmall, 0, 8, 16, arrayOfigits[7]);
	
}

void drawTime(unsigned char * obrazek, unsigned char (*arrayOfigits[4])[32]){
	int startYBig = 84;
	int startXBig = 20;
	int digitWidthBig = 16;
	int liczBig = 0;
	int spaceBig = 0;
	drawArray(obrazek, startYBig - digitWidthBig*(liczBig++) - spaceBig, startXBig, 16, 32, arrayOfigits[0]);
	drawArray(obrazek, startYBig - digitWidthBig*(liczBig++) - spaceBig, startXBig, 16, 32, arrayOfigits[1]);
	drawArray(obrazek, startYBig - digitWidthBig*(liczBig) - spaceBig, startXBig, 16, 32, bigColon);
	spaceBig+=8;
	drawArray(obrazek, startYBig - digitWidthBig*(liczBig++) - spaceBig, startXBig, 16, 32, arrayOfigits[2]);
	drawArray(obrazek, startYBig - digitWidthBig*(liczBig++) - spaceBig, startXBig, 16, 32, arrayOfigits[3]);
	
}

int main(){
	
	unsigned char (*smallDigits[10])[16] = {smallZero, smallOne, smallTwo, smallThree, smallFour, smallFive, smallSix, smallSeven, smallEight, smallNine};
    unsigned char (*bigDigits[10])[32] = {bigZero, bigOne, bigTwo, bigThree, bigFour, bigFive, bigSix, bigSeven, bigEight, bigNine};

	unsigned char (*arrayOfTables[8])[16] = {smallDigits[0],smallDigits[1],smallDigits[2],smallDigits[3],smallDigits[4],smallDigits[5],smallDigits[6],smallDigits[7]};
	drawDate(obrazek,arrayOfTables);
	unsigned char (*arrayOfTables2[8])[32] = {bigDigits[0],bigDigits[1],bigDigits[2],bigDigits[3]};
	drawTime(obrazek, arrayOfTables2);
	
	int configMode = 0; // 0: normal mode, 1: config mode
    int currentDigit = 0; // 0-11, liczba aktualnie edytowana
    int pulseState = 0; // 0: normal, 1: pulsing
    unsigned int pulseTime = 0; // Czas ostatniej zmiany stanu pulsowania
    int whichNext = 0; // zmiana na nastepny element
    unsigned char (*originalValue)[16] = arrayOfTables[currentDigit]; // wartosc dla migajacego elementu (zapisana wczesniej)

    uint8_t date[8] = {3,1,1,2,2,0,2,3};
	uint8_t time[4] = {2,1,3,7};

	OLED_display(obrazek);
	while(1){
        if(configMode){
            // W config mode
            if(Joystick_GetState() == JOYSTICK_CENTER){
                // Wyjdz z config mode
                configMode = 0;
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
            if(Joystick_GetState() == JOYSTICK_CENTER){
                // Wejdz w config mode
                configMode = 1;
                currentDigit = 0;
                pulseTime = SysTick_Get();
            }
        }
        drawDate(obrazek,arrayOfTables);
        drawTime(obrazek, arrayOfTables2);
        OLED_display(obrazek); 
    }

	return 0;	
}

