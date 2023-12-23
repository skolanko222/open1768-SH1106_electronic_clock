#include "image.h"
#include <stdio.h>
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

int main(){
	
	drawArray(obrazek, 0, 0, 3, 2, temp);
	
	consolDraw(obrazek);	

	return 0;	
}

