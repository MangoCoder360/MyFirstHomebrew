#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static u32 *xfb;
static GXRModeObj *rmode;


void Initialise() {
  
	VIDEO_Init();
	WPAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}


int main() {
 
	Initialise();
 
	printf("Ready!\n");
	
	while(1) {

		WPAD_ScanPads();

		u16 buttonsDown = WPAD_ButtonsDown(0);
		
		if( buttonsDown & WPAD_BUTTON_A ) {
			printf("Button A pressed.\n");
		}	
		
		u16 buttonsHeld = WPAD_ButtonsHeld(0);

		if (buttonsHeld & WPAD_BUTTON_A ) {
			printf("Button A is being held down.\n");
		}

		u16 buttonsUp = WPAD_ButtonsUp(0);

		if (buttonsUp & WPAD_BUTTON_A ) {
			printf("Button A released.\n");
		}

		/*if (PAD_StickY(0) > 18) {
			printf("Joystick moved up.\n");
		}
		
		if (PAD_StickY(0) < -18) {
			printf("Joystick moved down.\n");
		}*/

		if (buttonsDown & WPAD_BUTTON_HOME) {
			exit(0);
		}
	}
 
	return 0;
}