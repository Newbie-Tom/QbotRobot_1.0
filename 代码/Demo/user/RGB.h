#ifndef __RGB_H
#define __RGB_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"


#define RGB_GPIO_APBxClock_FUN		RCC_APB2PeriphClockCmd
#define RGB_GPIO_CLK				RCC_APB2Periph_GPIOA
#define RGB_GPIO_PORT				GPIOA
#define RGB_PIN						GPIO_Pin_1


#define RGB_LED_HIGH				GPIOA->BSRR = GPIO_Pin_1
#define RGB_LED_LOW					GPIOA->BRR = GPIO_Pin_1


//主时钟72M,每个nop是13.88ns
#define DELAY_13_88nS()		__nop()		//1个nop是13.8ns
#define DELAY_111nS()		DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS();DELAY_13_88nS() //8个nop是111ns
#define Delay_333nS()		DELAY_111nS();DELAY_111nS();DELAY_111nS()  //24个nop

//RGB结构体
typedef struct{
	u8 G;
	u8 R;
	u8 B;
	
}RGB_TypeDef;

#define PIXEL_NUM 2

void RGB_Init(void);
void RGB_LED_Write0(void);
void RGB_LED_Write1(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_Byte(uint8_t byte);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue(void);
void Delay_888nS(void);
void Delay_300uS(void);
void WS2812B_Reset(void);
void WS2812B_WriteByte(uint8_t dat);
void WS2812B_WriteColor(RGB_TypeDef *pColor);
void WS2812B_RefreshPixel(void);
void WS2812B_Test(void);
void WS2812B_Test2(void);
void Copy_Color(RGB_TypeDef *pDst,RGB_TypeDef *pScr);
void WS2812B_FillColor(u16 start,u16 end,RGB_TypeDef *pColor);
void WS2812B_MovePixel(u8 dir);







#endif

