
#include "stm32f4_discovery.h"

#include "sys.h"
#include "video.h"
#include "gdi.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "tyan-image.h"

#define VTOTAL	(VID_HSIZE+2)
extern u8	fb[VID_VSIZE][VTOTAL]  __attribute__((aligned(32)));


unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}


void deskchan(void){
	for (int i = 0; i < 14399; ++i) {
		love[i]=reverse(love[i]);
		love[i] = ~love[i];
	}


while(1){
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	char tyaninfo[88];
	sprintf(tyaninfo,"PIXELS %d,DECL %d",(256*450),(14400*8));
	gdiDrawTextEx(0,550,(pu8)tyaninfo,GDI_ROP_COPY);
	gdiBitBlt(NULL,100,20,250,450,(pu8)love,GDI_ROP_COPY);
	GPIO_SetBits(GPIOD,GPIO_Pin_13);
sysDelayMs(10);
}

return 0;
}







