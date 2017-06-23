#include "stm32f4_discovery.h"

#include "sys.h"
#include "video.h"
#include "gdi.h"
#include "string.h"
#include <stdlib.h>



extern void vidClearColumn(uint16_t x);

const u8 ball[] = { 0x01, 0x80,
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






//const u8 dummy[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#define LEFT 1
#define RIGHT 2
#define NOBODY 0
#define LEFT_UP_BTN GPIO_Pin_0
#define LEFT_DN_BTN GPIO_Pin_6
void playTennis(void){
int16_t YR=275,YL=275,BOARD_LEN = 50, BALLX=400,BALLY=300, VELX=2,VELY=1, BOARD_VEL=2;
uint8_t PLAYERWINS = NOBODY;

uint8_t PLAYWITHBOT = 1 ;
GPIO_InitTypeDef PORT_SETUP; //Создаем структуру, в которую заносим настройки
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Включить тактирование PORTD
PORT_SETUP.GPIO_Pin= LEFT_DN_BTN|LEFT_UP_BTN; //настройки для всех ножек
PORT_SETUP.GPIO_Mode = GPIO_Mode_IN; //ножка вход
PORT_SETUP.GPIO_PuPd = GPIO_PuPd_UP;
PORT_SETUP.GPIO_Speed = GPIO_Speed_2MHz; //скорость 2мгц

GPIO_Init( GPIOD , &PORT_SETUP); //записать настройки

while(1){

	gdiBitBlt(NULL,BALLX,BALLY,16,16,(pu8) ball,GDI_ROP_XOR);
	vidClearColumn(2);
	vidClearColumn(VID_PIXELS_X-2);

if(--BALLY<=0){
VELY=(-VELY);
}
if(++BALLY+16>=VID_PIXELS_Y){
VELY=-(VELY);
}
if(--BALLX<=2){
if(BALLY+8>YL && (BALLY+8)<(YL+50)){
VELX=(-VELX);
}else{
PLAYERWINS=RIGHT;
}
}
if(++BALLX+16>=VID_PIXELS_X-2){
if(BALLY+8>YR && (BALLY+8)<(YR+50)){
VELX=(-VELX);
}else{
PLAYERWINS=LEFT;
}
}

BALLX+=VELX;
BALLY+=VELY;
gdiBitBlt(NULL,BALLX,BALLY,16,16,(pu8) ball,GDI_ROP_COPY);

gdiLine(NULL,2,YL,2,YL+BOARD_LEN,GDI_ROP_COPY);
gdiLine(NULL,VID_PIXELS_X-2,YR,VID_PIXELS_X-2,YR+BOARD_LEN,GDI_ROP_COPY);
if(GPIO_ReadInputDataBit(GPIOD,LEFT_DN_BTN)==0&&GPIO_ReadInputDataBit(GPIOD,LEFT_UP_BTN)==1)
{
YL-=BOARD_VEL;
}else if(GPIO_ReadInputDataBit(GPIOD,LEFT_UP_BTN)==0&&GPIO_ReadInputDataBit(GPIOD,LEFT_DN_BTN)==1)
{
YL+=BOARD_VEL;
}


if(PLAYWITHBOT){
if(BALLY>YR+25){YR+=BOARD_VEL;}
else if(BALLY<YR+25){YR-=BOARD_VEL;}
}
sysDelayMs(30);
while(PLAYERWINS==LEFT)
{
gdiDrawTextEx(180, 40, (pu8)"LEFT WINS", GDI_ROP_COPY);
}
while(PLAYERWINS==RIGHT)
{
gdiDrawTextEx(180, 40, (pu8)"RIGHT WINS", GDI_ROP_COPY);
}



}


}
