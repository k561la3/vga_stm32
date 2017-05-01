#include "stm32f4_discovery.h"

#include "sys.h"
#include "video.h"
#include "gdi.h"
#include "string.h"
#include <stdlib.h>




const u8	ball[] = {	0x01, 0x80,
								0x07, 0xe0,
								0x1f, 0xf8,
								0x3f, 0xfc,
								0x3f, 0xfc,
								0x7f, 0xfe,
								0x7f, 0xfe,
								0xff, 0xff,
								0xff, 0xff,
								0x7f, 0xfe,
								0x7f, 0xfe,
								0x3f, 0xfc,
								0x3f, 0xfc,
								0x1f, 0xf8,
								0x07, 0xe0,
								0x01, 0x80};
#define LEFT 1
#define RIGHT 2;
#define NOBODY 0
void game(void){
	int16_t YR=275,YL=275,BOARD_LEN = 50, BALLX=400,BALLY=300, VELX=1,VELY=1;
uint8_t PLAYERWINS = NOBODY;



while(1){
	vidClearScreen();
	gdiLine(NULL,2,YL,2,YL+BOARD_LEN,GDI_ROP_COPY);
	gdiLine(NULL,VID_PIXELS_X-2,YR,VID_PIXELS_X-2,YR+BOARD_LEN,GDI_ROP_COPY);
	gdiBitBlt(NULL,BALLX,BALLY,16,16,(pu8) ball,GDI_ROP_COPY);
	if(--BALLY<=0){
		VELY=(-VELY);
		}
	if(++BALLY+16>=VID_PIXELS_Y){
			VELY=-(VELY);
			}
	if(--BALLX<=2){
			if(BALLY>YL && (BALLY+16)<(YL+50)){
				VELX=(-VELX);
			}else{
				PLAYERWINS=RIGHT;
			}
			}
	if(++BALLX+16>=VID_PIXELS_X-2){
				if(BALLY>YR && (BALLY+16)<(YR+50)){
					VELX=(-VELX);
				}else{
					PLAYERWINS=LEFT;
				}
				}

	BALLX+=VELX;
	BALLY+=VELY;
}


}






























