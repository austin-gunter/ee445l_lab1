#include "fixed.h"
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

uint32_t x_min = 0;
uint32_t x_max = 0;
uint32_t y_min = 0;
uint32_t y_max = 0;

void ST7735_sDecOut3(int32_t n){
	if (n < -9999 || n > 9999){						//If n is outside range, print a space (for organization) and then *.***
		fputc(32, 0);
		fputc(42, 0);	
		fputc(46, 0);
		fputc(42, 0);	
		fputc(42, 0);
		fputc(42, 0);
		return;
	}
	
	if (n < 0){														//If n is negative, print - and then change it to positive to make ASCII calculations easier.
		fputc(45, 0);
		n *= -1;
		} else {														//Otherwise, print a space so the display looks more organized.
		fputc(32, 0);
	}
	
	int32_t x = 1000;
	int32_t z = n % 1000;
	int32_t ASCII = 48;										//ASCII value for a single positive digit = digit + 48
	
	fputc((n/x + ASCII), 0);							//Print first digit of fixed point number
	fputc(46, 0);
	for (int i = 0; i < 3; i++){					//Print rest of digits
		x /= 10;
		fputc(z/x + ASCII, 0);
		z = n % x;
	}
};

void ST7735_uBinOut8(uint32_t n){
	if (n >= 256000){											//If n is out of range, print ***.**
	fputc(42, 0);
	fputc(42, 0);
	fputc(42, 0);
	fputc(46, 0);
	fputc(42, 0);
	fputc(42, 0);
	return;
	}
	
	n *= 100;															//Multiply values by 100 in order to not lose precision when dividing by 256.
	int32_t x = 2560000;									//	32 bits to use, we have plenty of space.
	int32_t z = n % 2560000;
	int32_t ASCII = 48;										//ASCII value for a single positive digit = digit + 48
	
	fputc(n/x + ASCII, 0);								//Print first digit
	for (int i = 0; i < 2; i++){					//Print next 2 digits
		x /= 10;
		fputc(z/x + ASCII, 0);
		z = n % x;
	}
	fputc(46, 0);													//Print .
	for (int i = 0; i < 2; i++){					//Print last 2 digits
		x /= 10;
		fputc(z/x + ASCII, 0);
		z = n % x;
	}
};

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
	x_min = minX;
	x_max = maxX;
	y_min = minY;
	y_max = maxY;
	ST7735_InitR(INITR_REDTAB);
	ST7735_OutString(title);
	ST7735_PlotClear(minY, maxY);

//i goes from 0 to 127, scales X
//uint32_t i = (127*(x-minX))/(maxX-minX);
//j goes from 32 to 159, scales Y
//uint32_t j = 32+(127*(maxY-y))/(maxY-minY);
};

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
	for(int i = 0; i < num; i++){
		int16_t x = (((bufX[i] - x_min)*127))/(x_max - x_min);
		int16_t y = 32 + (((y_max - bufY[i])*127))/(y_max - y_min);
		ST7735_DrawPixel(x, y, ST7735_BLUE);
	}
};
