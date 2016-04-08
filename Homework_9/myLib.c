#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void drawImage3(int r, int c, int width, int height, const u16* image) {
	for (int row = 0; row < height; row++) {
		DMA[3].src = &image[width * row];
		DMA[3].dst = &videoBuffer[OFFSET(row + r, c, 240)];
		DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
	}
}

void fillScreen3() {
	volatile int color = (int) CYAN;
	DMA[3].src = &color; //src is where in the file you want to start copying
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_SOURCE_FIXED | DMA_ON | 240 * 160;
}

void drawChar(int row, int col, char ch, u16 color) {
	for(int r=0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, u16 color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

void setPixel(int r, int c, unsigned short color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


