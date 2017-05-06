#include "stm32f4_discovery.h"

#include "sys.h"
#include "video.h"
#include "gdi.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

#define GREEN			LED4_PIN
#define ORANGE			LED3_PIN
#define RED				LED5_PIN
#define BLUE			LED6_PIN
#define ALL_LEDS		(GREEN | ORANGE | RED | BLUE)		// all leds
#define LEDS_GPIO_PORT (GPIOD)

#define  LED_ON(x)        GPIO_SetBits(LEDS_GPIO_PORT, x)
#define  LED_OFF(x)       GPIO_ResetBits(LEDS_GPIO_PORT, x)




#define VTOTAL	(VID_HSIZE+2)
extern u8	fb[VID_VSIZE][VTOTAL]  __attribute__((aligned(32)));
extern void vidClearColumn(uint16_t x);

const u8 ataball[] = { 0x01, 0x80,
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

#define BOARD_LEN 50
#define BOARD_SPD 2
i8 BALLSPEEDX =1;
i8 BALLSPEEDY = -1;
u8 LOSER =1;
u16 BOARD_X = 375,BLX=400,BLY=300;

const u8 block[] = {0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0,
					0xFF,0xFF,0xFF,0xFF,0};

const u8 clrball[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define LEFT_BTN GPIO_Pin_0
#define RIGHT_BTN GPIO_Pin_6

void setupAtari(void){
	for (int blocky = 0; blocky < 5; blocky++) {
		for (int blockx = 0; blockx < 20; blockx++) {
			gdiBitBlt(NULL,blockx*40,blocky*15,40,12, (pu8)block,GDI_ROP_COPY);
		}
	}
	GPIO_InitTypeDef PORT_SETUP; //Создаем структуру, в которую заносим настройки
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Включить тактирование PORTD
	PORT_SETUP.GPIO_Pin= LEFT_BTN|RIGHT_BTN; //настройки для всех ножек
	PORT_SETUP.GPIO_Mode = GPIO_Mode_IN; //ножка вход
	PORT_SETUP.GPIO_PuPd = GPIO_PuPd_UP;
	PORT_SETUP.GPIO_Speed = GPIO_Speed_2MHz; //скорость 2мгц

	GPIO_Init( GPIOD , &PORT_SETUP); //записать настройки
}


void playAtari(void){
	setupAtari();
while(LOSER){
	gdiBitBlt(NULL,BLX,BLY,16,16,(pu8) clrball,GDI_ROP_COPY);
	vidClearRow(VID_PIXELS_Y-2);

	gdiLine(NULL,BOARD_X,VID_PIXELS_Y-2,BOARD_X+BOARD_LEN,VID_PIXELS_Y-2,GDI_ROP_COPY);
	if(GPIO_ReadInputDataBit(GPIOD,LEFT_BTN)==0&&GPIO_ReadInputDataBit(GPIOD,RIGHT_BTN)==1)
	{
	BOARD_X-=BOARD_SPD;
	}else if(GPIO_ReadInputDataBit(GPIOD,RIGHT_BTN)==0&&GPIO_ReadInputDataBit(GPIOD,LEFT_BTN)==1)
	{
	BOARD_X+=BOARD_SPD;
	}
	if(++BLY>=VID_PIXELS_Y-18){
		if ((BLX<=BOARD_X+34) && (BLX>=BOARD_X)) {
			BALLSPEEDY=(-BALLSPEEDY);
		}else{
			LOSER = 0;
		}
	}

if((--BLX<=8)){
BALLSPEEDX=(-BALLSPEEDX);
LED_ON(ORANGE);
}
if((++BLX>=784)){
BALLSPEEDX=(-BALLSPEEDX);
LED_ON(RED);
}
if(--BLY<=1){
BALLSPEEDY=(-BALLSPEEDY);
LED_ON(BLUE);
}
//char debug_buf[32];
//sprintf(debug_buf,"DIGIT = %d",fb[--BLY][(BLX+8)/8]);
//gdiDrawTextEx(500, 300, (pu8)debug_buf, GDI_ROP_COPY);
BLX+=BALLSPEEDX;
BLY+=BALLSPEEDY;
if((u8)(fb[BLY-1][(BLX+8)/8])==(u8)255){BALLSPEEDY=(-BALLSPEEDY);
gdiBitBlt(NULL,(BLX/40)*40,(BLY/15)*15,40,12, (pu8)block,GDI_ROP_XOR);
LED_ON(GREEN);
}


gdiBitBlt(NULL,BLX,BLY,16,16,(pu8) ataball,GDI_ROP_COPY);
sysDelayMs(20);
}
gdiDrawTextEx(500, 300, (pu8)"U R LOSER SUCK", GDI_ROP_COPY);
while(1){}

}
