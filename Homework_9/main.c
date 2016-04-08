#include "myLib.h"
#include "frogcrossing.h"
#include "frog.h"
#include "logs.h"
#include "cyan.h"
#include "mushroom.h"
#include "deadfrog.h"
#include "youwin.h"

enum GBAState {
	START,
	START_NODRAW,
	//states for levels, end state
	LEVEL1,
	LEVEL1_NODRAW,
	END,
	END_NODRAW,
	WIN,
	WIN_NODRAW
};

typedef struct {
	int row;
	int col;
	int dRow;
	int dCol;
	int width;
	int height;
	u16* image;
} OBJECT;


int main() {

	// Draw the home screen
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	enum GBAState state = START;
	OBJECT log1 = {120, 0, 0, 1, LOGS_WIDTH, LOGS_HEIGHT, (u16*) logs};
	OBJECT log2 = {80, 0, 0, 3, LOGS_WIDTH, LOGS_HEIGHT, (u16*) logs};
	OBJECT log3 = {40, 0, 0, 3, LOGS_WIDTH, LOGS_HEIGHT, (u16*) logs};
	OBJECT froggy = {140, 120, 1, 1, FROG_WIDTH, FROG_HEIGHT, (u16*) frog};
	OBJECT cyanRect1 = {log1.row, log1.col - log1.dCol, 0, log1.dCol, log1.width, log1.height, (u16*) cyan};
	OBJECT cyanRect2 = {log2.row, log2.col - log2.dCol, 0, log2.dCol, log2.width, log2.height, (u16*) cyan};
	OBJECT cyanRect3 = {log3.row, log3.col - log3.dCol, 0, log3.dCol, log3.width, log3.height, (u16*) cyan};
	OBJECT house1 = {10, 60 - MUSHROOM_WIDTH/2, 0, 0, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, (u16*) mushroom};
	OBJECT house2 = {10, 120 - MUSHROOM_WIDTH/2, 0, 0, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, (u16*) mushroom};
	OBJECT house3 = {10, 180 - MUSHROOM_WIDTH/2, 0, 0, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, (u16*) mushroom};
	while(1) {
		// all updates before waitForVblank, drawing after
		// moves the log
		if (log1.col + log1.width <= 239) {
			log1.col = log1.col + log1.dCol;
		} else {
			log1.col = 0;
		}
		if (log2.col + log2.width <= 239) {
			log2.col = log2.col + log2.dCol;
		} else {
			log2.col = 0;
		}
		if (log3.col + log3.width <= 239) {
			log3.col = log3.col + log3.dCol;
		} else {
			log3.col = 0;
		}
		//moves the frog
		if (KEY_DOWN_NOW(BUTTON_UP) && state == LEVEL1) {
			if (froggy.row - froggy.dRow > 0)
				froggy.row = froggy.row - froggy.dRow;
		} else if (KEY_DOWN_NOW(BUTTON_DOWN) && state == LEVEL1) {
			if (froggy.row + froggy.dRow + froggy.height < 160)
				froggy.row = froggy.row + froggy.dRow;
		} else if (KEY_DOWN_NOW(BUTTON_LEFT) && state == LEVEL1) {
			if (froggy.col - froggy.dCol > 0)
				froggy.col = froggy.col - froggy.dCol;
		} else if (KEY_DOWN_NOW(BUTTON_RIGHT) && state == LEVEL1) {
			if (froggy.col + froggy.dCol + froggy.width < 240)
				froggy.col = froggy.col + froggy.dCol;
		}
		//handles collision
		if ((froggy.col + froggy.width >= log1.col) && (froggy.col <= log1.col + log1.width) && (froggy.row + froggy.height >= log1.row) && (froggy.row <= log1.row + log1.height)) {
			state = END;
		}
		if ((froggy.col + froggy.width >= log2.col) && (froggy.col <= log2.col + log2.width) && (froggy.row + froggy.height >= log2.row) && (froggy.row <= log2.row + log2.height)) {
			state = END;
		}
		if ((froggy.col + froggy.width >= log3.col) && (froggy.col <= log3.col + log3.width) && (froggy.row + froggy.height >= log3.row) && (froggy.row <= log3.row + log3.height)) {
			state = END;
		}
		if ((froggy.col + froggy.width >= house1.col) && (froggy.col <= house1.col + house1.width) && (froggy.row + froggy.height >= house1.row) && (froggy.row <= house1.row + house1.height)) {
			state = WIN;
		}
		if ((froggy.col + froggy.width >= house2.col) && (froggy.col <= house2.col + house2.width) && (froggy.row + froggy.height >= house2.row) && (froggy.row <= house2.row + house2.height)) {
			state = WIN;
		}
		if ((froggy.col + froggy.width >= house3.col) && (froggy.col <= house3.col + house3.width) && (froggy.row + froggy.height >= house3.row) && (froggy.row <= house3.row + house3.height)) {
			state = WIN;
		}
		
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = START;
		}
		
		waitForVblank();
		switch(state) {
			case START:
				froggy.row = 140;
				froggy.col = 120;
				drawImage3(0, 0, 240, 160, frogcrossing);
				drawString(130, 60, "PRESS ENTER TO PLAY", BLACK);
				state = START_NODRAW;
				break;
			case START_NODRAW:
				while (KEY_DOWN_NOW(BUTTON_START)) {
					state = LEVEL1;
				}
				break;
			case LEVEL1:
				fillScreen3();
				drawImage3(10, 60 - MUSHROOM_WIDTH/2, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, mushroom);
				drawImage3(10, 120 - MUSHROOM_WIDTH/2, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, mushroom);
				drawImage3(10, 180 - MUSHROOM_WIDTH/2, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, mushroom);
				drawImage3(froggy.row, froggy.col, froggy.width, froggy.height, froggy.image);
				drawImage3(log1.row, log1.col, log1.width, log1.height, log1.image);
				drawImage3(cyanRect1.row, cyanRect1.col, cyanRect1.width, cyanRect1.height, cyanRect1.image);
				drawImage3(log2.row, log2.col, log2.width, log2.height, log2.image);
				drawImage3(cyanRect2.row, cyanRect2.col, cyanRect2.width, cyanRect2.height, cyanRect2.image);
				drawImage3(log3.row, log3.col, log3.width, log3.height, log3.image);
				drawImage3(cyanRect3.row, cyanRect3.col, cyanRect3.width, cyanRect3.height, cyanRect3.image);
				state = LEVEL1_NODRAW;
				break;
			case LEVEL1_NODRAW:
				if (KEY_DOWN_NOW(BUTTON_A)) {
					state = START;
				} else {
					state = LEVEL1;
				}
				break;
			case END:
				drawImage3(0, 0, DEADFROG_WIDTH, DEADFROG_HEIGHT, deadfrog);
				drawString(130, 60, "ENTER TO PLAY AGAIN", MAGENTA);
				state = END_NODRAW;
				break;
			case END_NODRAW:
				while (KEY_DOWN_NOW(BUTTON_START)) {
					state = START;
				}
				break;
			case WIN:
				drawImage3(0, 0, YOUWIN_WIDTH, YOUWIN_HEIGHT, youwin);
				drawString(130, 100, "PRESS ENTER", MAGENTA);
				state = WIN_NODRAW;
				break;
			case WIN_NODRAW:
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
				break;
		}
	}	
	return 0;
}



