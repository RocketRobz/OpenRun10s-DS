#include <nds.h>
#include <sys/stat.h>
#include "gl2d.h"
#include "gif.hpp"
#include "sound.h"
#include <maxmod9.h>
#include "tonccpy.h"

#include "graphics/graphics.h"

#include "logo_rocketrobz.h"

#include "player.h"
#include "level.h"

static int subScreenMode = 0;

extern int language;

extern bool wideScreen;
extern bool fadeType;
extern bool fadeColor;
extern bool controlTopBright;
extern bool controlBottomBright;
extern int screenMode;
extern bool run3DMode;

static bool inited = false;
static int waitTime = 0;
static int waitTimeLimit = (60*3)+30;
static int frameDelay = 0;
static int frameRun = 0;

void logoScreen(void) {
	if (!inited) {
		if (subScreenMode == 1) {
			bg3Sub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
			decompress(logo_rocketrobzBitmap, bgGetGfxPtr(bg3Sub), LZ77Vram);
			toncset16(BG_PALETTE, 0xFFFF, 256*sizeof(u16));
			fadeType = true;
			inited = true;

			Gif gmode("nitro:/gmodeLogo.gif", true, true);
			snd().playGmode();
			while (frameRun < 24) {
				if (frameDelay == 0) {
					gmode.displayFrame();
					gmode.resume();
					frameRun++;
				}
				frameDelay++;
				if (frameDelay > 3) frameDelay = 0;
				swiWaitForVBlank();
			}
			waitTimeLimit = 90;
		} else {
			Gif go("nitro:/logo_Go.gif", true, false);
			Gif gamebridge("nitro:/logo_Gamebridge.gif", false, false);
			go.displayFrame();
			gamebridge.displayFrame();
			controlBottomBright = false;
			fadeType = true;
			inited = true;
		}
	}

	if (subScreenMode == 0) {
		if (waitTime > 120) {
			controlTopBright = true;
		} else if (waitTime > 75) {
			controlBottomBright = true;
			fadeType = true;
		} else if (waitTime > 30) {
			controlTopBright = false;
			fadeType = false;
		}
	}

	if (waitTime == waitTimeLimit) {
		if (subScreenMode == 1) {
			fadeColor = false;
			fadeType = false;
			while (!screenFadedOut());

			videoSetMode(MODE_5_3D);

			// Initialize OAM to capture 3D scene
			extern void initSubSprites(void);
			initSubSprites();

			// The sub background holds the top image when 3D directed to bottom
			bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

			glScreen2D();

			// Clear the GL texture state
			glResetTextures();

			vramSetBankA(VRAM_A_TEXTURE);
			vramSetBankB(VRAM_B_TEXTURE);
			vramSetBankE(VRAM_E_TEX_PALETTE);
			vramSetBankF(VRAM_F_TEX_PALETTE_SLOT4);
			vramSetBankG(VRAM_G_TEX_PALETTE_SLOT5); // 16Kb of palette ram, and font textures take up 8*16 bytes.

			playerGraphicLoad();
			levelGraphicLoad();

			run3DMode = true;
			screenMode = 1;
		} else {
			fadeType = false;
			while (!screenFadedOut());
			subScreenMode = 1;
			waitTime = 0;
			inited = false;
		}
	}
	waitTime++;
}

void renderLogo(void) {
}
