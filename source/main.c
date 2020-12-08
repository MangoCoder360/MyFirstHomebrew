#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static u32 *xfb;
static GXRModeObj *rmode;

// Wiimote IR
ir_t ir;

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

void DrawHLine (int x1, int x2, int y, int color) {
	int i;
	y = 320 * y;
	x1 >>= 1;
	x2 >>= 1;
	for (i = x1; i <= x2; i++) {
		u32 *tmpfb = xfb;
		tmpfb[y+i] = color;
	}
}

void DrawVLine (int x, int y1, int y2, int color) {
	int i;
	x >>= 1;
	for (i = y1; i <= y2; i++) {
		u32 *tmpfb = xfb;
		tmpfb[x + (640 * i) / 2] = color;
	}
}

void DrawBox (int x1, int y1, int x2, int y2, int color) {
	DrawHLine (x1, x2, y1, color);
	DrawHLine (x1, x2, y2, color);
	DrawVLine (x1, y1, y2, color);
	DrawVLine (x2, y1, y2, color);
}

int main() {
 
	Initialise();
 
	printf("Ready!\n");
	
	while(1) {

		WPAD_ScanPads();

		u16 buttonsDown = WPAD_ButtonsDown(0);

		// IR Movement
		WPAD_IR(0, &ir);
		
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

		VIDEO_ClearFrameBuffer (rmode, xfb, COLOR_BLACK);
		
		DrawBox (ir.x, ir.y, ir.x + 1, ir.y + 1, COLOR_WHITE);
		
		VIDEO_WaitVSync();
	}
 
	return 0;
}