#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>

#include <string.h>
#include <unistd.h>
#include <maxmod9.h>
#include "gl2d.h"
#include "sound.h"

#include "graphics/graphics.h"
#include "graphics/fontHandler.h"
#include "nitrofs.h"

#include "logos.h"
#include "mainmenu.h"
#include "level.h"

bool useTwlCfg = false;
int language = 0;

bool wideScreen = false;
bool fadeType = false;				// false = out, true = in
bool fadeSpeed = true; // false = slow, true = fast
bool fadeColor = true;
int screenMode = 0;

//---------------------------------------------------------------------------------
void stop (void) {
//---------------------------------------------------------------------------------
	while (1) {
		swiWaitForVBlank();
	}
}

//---------------------------------------------------------------------------------
void doPause(int x, int y) {
//---------------------------------------------------------------------------------
	// iprintf("Press start...\n");
	printSmall(false, x, y, "Press start...");
	while(1) {
		scanKeys();
		if(keysDown() & KEY_START)
			break;
	}
	scanKeys();
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// overwrite reboot stub identifier
	// so tapping power on DSi returns to DSi menu
	extern u64 *fake_heap_end;
	*fake_heap_end = 0;

	defaultExceptionHandler();

	useTwlCfg = (isDSiMode() && (*(u8*)0x02000400 & 0x0F) && (*(u8*)0x02000404 == 0));
	wideScreen = (strcmp(argv[1], "wide") == 0);

	language = (useTwlCfg ? *(u8*)0x02000406 : PersonalData->language);

	bool fatInited = fatInitDefault();
	bool nitroFSInited = nitroFSInit(argv[0]);

	if (!fatInited && !nitroFSInited) {
		consoleDemoInit();
		iprintf("fatInitDefault failed!");
		stop();
	}

	if (!nitroFSInited) {
		consoleDemoInit();
		iprintf("NitroFS init failed!");
		stop();
	}

	snd();	
	graphicsInit();
	//fontInit();

	while (1) {
		clearText();
		switch (screenMode) {
			case 0:
			default:
				logoScreen();
				break;
			case 1:
				menuScreen();
				break;
			case 2:
				levelMode();
				break;
		}
		snd().updateStream();
		swiWaitForVBlank();
	}

	return 0;
}
