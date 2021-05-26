#include <nds.h>
#include "gl2d.h"
#include "sound.h"

#include "graphics/fontHandler.h"
#include "graphics/graphics.h"
#include "tonccpy.h"

#include "player.h"

#include "tiles.h"

#include "course1.h"

extern bool wideScreen;
extern bool fadeType;
extern int screenMode;

static bool inited = false;

int cameraXpos = 0;
//int cameraYpos = 0;

u8 mapData[64*16] = {23};

extern int playerX, playerY;

static int tileTexID;
glImage tileImage[(128 / 16) * (128 / 16)];

void levelGraphicLoad(void) {
	tileTexID = glLoadTileSet(tileImage, // pointer to glImage array
							16, // sprite width
							16, // sprite height
							128, // bitmap width
							128, // bitmap height
							GL_RGB16, // texture type for glTexImage2D() in videoGL.h
							TEXTURE_SIZE_128, // sizeX for glTexImage2D() in videoGL.h
							TEXTURE_SIZE_128, // sizeY for glTexImage2D() in videoGL.h
							GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
							16, // Length of the palette to use (16 colors)
							(u16*) tilesPal, // Load our 16 color tiles palette
							(u8*) tilesBitmap // image data generated by GRIT
							);
}

bool isSolidTile(u8 tile) {
	switch (tile) {
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			return true;
		default:
			break;
	}
	return false;
}

void loadLevel(u8* orgMapData) {
	tonccpy(&mapData, orgMapData, 64*16);

	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 16; y++) {
			if (mapData[(y*64)+x] == '1') {
				setPlayerPosition(x*12, (y*12)-12);
				mapData[(y*64)+x] = 23;
			}
		}
	}
}

void levelMode(void) {
	if (!inited) {
		initPlayers();
		loadLevel(course1);

		fadeType = true;
		inited = true;
		snd().loadStream("nitro:/BGM01.raw", 16364, true);
		snd().beginStream();
	}

	scanKeys();
	int pressed = keysDown();
	int held = keysHeld();
	
	playerLoop(pressed, held);

	if (pressed & KEY_START) {
		fadeType = false;
		snd().fadeOutStream();
		while (!screenFadedOut());
		snd().stopStream();
		screenMode = 1;
		inited = false;
	}
}

void levelGraphicDisplay(void) {
	glBoxFilled(0, 0, (wideScreen ? 308 : 256), 192, RGB15(31, 31, 31));
	/*for (int x = 0; x < 256; x += 16) {
		for (int y = 0; y < 192; y += 16) {
			glSprite(x, y, GL_FLIP_NONE, &tileImage[bgTile]);
		}
	}*/
	if (playerX>((wideScreen ? 308 : 256)/2)) {
		cameraXpos = playerX-((wideScreen ? 308 : 256)/2);
		if (cameraXpos > (64*12)-(wideScreen ? 308 : 256)) {
			cameraXpos = (64*12)-(wideScreen ? 308 : 256);
		}
	} else {
		cameraXpos = 0;
	}
	//if (mapVsize>12 && playerY>192/2) {
	//	cameraYpos = playerY-(192/2);
	/*	if (cameraYpos > (mapVsize*16)-192) {
			cameraYpos = (mapVsize*16)-192;
		}
	} else {
		cameraYpos = 0;
	}*/
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 16; y++) {
			if (mapData[(y*64)+x] != 23) {
				glSprite((x*12)-cameraXpos, (y*12), GL_FLIP_NONE, &tileImage[mapData[(y*64)+x]]);
			}
			if (mapData[(y*64)+(x-1)] == 11 && (mapData[(y*64)+x] == 13 || mapData[(y*64)+x] == 15)) {
				glSprite((x*12)-cameraXpos, (y*12), GL_FLIP_NONE, &tileImage[25]);
			}
			if ((mapData[(y*64)+x] == 14 || mapData[(y*64)+x] == 15) && mapData[(y*64)+(x+1)] == 11) {
				glSprite((x*12)-cameraXpos, (y*12), GL_FLIP_NONE, &tileImage[26]);
			}
			if (mapData[(y*64)+x] == 18 && mapData[(y*64)+(x+1)] == 11) {
				glSprite((x*12)-cameraXpos, (y*12), GL_FLIP_NONE, &tileImage[32]);
			}
			if (mapData[(y*64)+(x-1)] == 11 && mapData[(y*64)+x] == 17) {
				glSprite((x*12)-cameraXpos, (y*12), GL_FLIP_NONE, &tileImage[28]);
			}
		}
	}
	renderPlayer(true);
}

void levelGraphicDisplayBottom(void) {
	glBoxFilled(0, 0, 256, 192, RGB15(31, 31, 31));
	renderPlayer(false);
}