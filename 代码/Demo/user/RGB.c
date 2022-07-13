#include "rgb.h"

RGB_TypeDef PixelBuf[PIXEL_NUM];

u8 flag;

void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RGB_GPIO_APBxClock_FUN(RGB_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RGB_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(RGB_GPIO_PORT,&GPIO_InitStructure);
}


void RGB_LED_Write0(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}
 

void RGB_LED_Write1(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset(void)
{
	RGB_LED_LOW;
	delay_us(80);
}
 
void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;
 
	for(i=0;i<8;i++)
		{
			if(byte&0x80)
				{
					RGB_LED_Write1();
			}
			else
				{
					RGB_LED_Write0();
			}
		byte <<= 1;
	}
}


void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}
 
 
 
 
void RGB_LED_Red(void)
{
	 uint8_t i;
	for(i=0;i<1;i++)
		{
			RGB_LED_Write_24Bits(0, 0xff, 0);
	}
}
 
void RGB_LED_Green(void)
{
	uint8_t i;
 
	for(i=0;i<1;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}
 
void RGB_LED_Blue(void)
{
	uint8_t i;
 
	for(i=0;i<1;i++)
		{
			RGB_LED_Write_24Bits(0, 0, 0xff);
	}
}


/*---------------------------------------------------------------------------
@Function   :Delay_888nS
@Description:检测开关初始化
@Input      :无
@Retrun     :无
@Others     :T1H T0L 时间在  580ns~1.6us,取850ns   实测855us
----------------------------------------------------------------------------*/
void Delay_888nS(void)
{
	//进入函数所用的时间约为310ns
	DELAY_111nS();
	DELAY_111nS();
	Delay_333nS();
	Delay_333nS();
}

/*---------------------------------------------------------------------------
@Function   :Delay_300uS
@Description:
@Input      :无
@Retrun     :无
----------------------------------------------------------------------------*/
void Delay_300uS(void)
{
	u16 i;
	i=901;
	while(i--)
	{
		Delay_333nS();
	}
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_Reset
@Description:复位
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_Reset(void)          //复位函数
{
	RGB_LED_LOW;
	delay_us(300);
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_WriteByte
@Description:写一个字节
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_WriteByte(uint8_t dat)
{
	u8 i;
	for (i=0;i<8;i++)
	{
		//先发送高位
		if (dat & 0x80) //1
		{
			RGB_LED_HIGH;
			Delay_888nS();
			RGB_LED_LOW;				
			Delay_333nS();
		}
		else	//0
		{
			RGB_LED_HIGH;
			Delay_333nS(); //T0H
			RGB_LED_LOW;	
			Delay_888nS(); //T0L
		}
		dat<<=1;
	}
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_WriteColor
@Description:写入1个24bit颜色数据
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_WriteColor(RGB_TypeDef *pColor)
{
	WS2812B_WriteByte(pColor->G);
	WS2812B_WriteByte(pColor->R);
	WS2812B_WriteByte(pColor->B);
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_RefreshPixel
@Description:更新显示
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_RefreshPixel(void)
{
	u8 i;

	for(i=0;i<PIXEL_NUM;i++)
	{
		WS2812B_WriteColor(&PixelBuf[i]);
	}
}

void WS2812B_Test(void)
{
	u8 i;
	RGB_TypeDef temp;
	temp.B = 0x50;
	temp.R = 0x60;
	temp.G = 0x70;

	for(i=0;i<2;i++)
	{
		WS2812B_WriteColor(&temp);
	}
}

//测试延时时间
void WS2812B_Test2(void)
{
	RGB_LED_HIGH;
	Delay_888nS();
	RGB_LED_LOW;
}

//============================================================================
void Copy_Color(RGB_TypeDef *pDst,RGB_TypeDef *pScr)
{
	pDst->R = pScr->R;
	pDst->G = pScr->G;
	pDst->B = pScr->B;
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_FillColor
@Description:填充颜色
@Input      :start：开始位置;end:结束信置;pColor:颜色值
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_FillColor(u16 start,u16 end,RGB_TypeDef *pColor)
{
	if (start > end) //交换位置
	{
		u16 temp;
		temp = start;
		start = end;
		end = temp;
	}

	if (start >= PIXEL_NUM)
		return; //超出范围
	if (end >= PIXEL_NUM)
		end = PIXEL_NUM-1;

	//填充颜色值
	while(start <= end)
	{
		Copy_Color(&PixelBuf[start],pColor);
		start++;
	}
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_MovePixel
@Description:循环移动像素颜色
@Input      :dir:方向;
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_MovePixel(u8 dir)
{
	RGB_TypeDef temp;
	u8 i;

	if (dir) //向左移动
	{
		Copy_Color(&temp,&PixelBuf[PIXEL_NUM-1]);

		i = PIXEL_NUM-1;
		while(i)
		{
			 Copy_Color(&PixelBuf[i],&PixelBuf[i-1]);
			 i--;
		}
		Copy_Color(&PixelBuf[0],&temp);
	}
	else  //向右移动
	{
		Copy_Color(&temp,&PixelBuf[0]);

		i = 0;
		while(i < (PIXEL_NUM-1))
		{
			Copy_Color(&PixelBuf[i],&PixelBuf[i+1]);
			i++;
		}
		Copy_Color(&PixelBuf[PIXEL_NUM-1],&temp);
	}
}


