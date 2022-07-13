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
@Description:��⿪�س�ʼ��
@Input      :��
@Retrun     :��
@Others     :T1H T0L ʱ����  580ns~1.6us,ȡ850ns   ʵ��855us
----------------------------------------------------------------------------*/
void Delay_888nS(void)
{
	//���뺯�����õ�ʱ��ԼΪ310ns
	DELAY_111nS();
	DELAY_111nS();
	Delay_333nS();
	Delay_333nS();
}

/*---------------------------------------------------------------------------
@Function   :Delay_300uS
@Description:
@Input      :��
@Retrun     :��
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
@Description:��λ
@Input      :��
@Retrun     :��
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_Reset(void)          //��λ����
{
	RGB_LED_LOW;
	delay_us(300);
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_WriteByte
@Description:дһ���ֽ�
@Input      :��
@Retrun     :��
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_WriteByte(uint8_t dat)
{
	u8 i;
	for (i=0;i<8;i++)
	{
		//�ȷ��͸�λ
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
@Description:д��1��24bit��ɫ����
@Input      :��
@Retrun     :��
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
@Description:������ʾ
@Input      :��
@Retrun     :��
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

//������ʱʱ��
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
@Description:�����ɫ
@Input      :start����ʼλ��;end:��������;pColor:��ɫֵ
@Retrun     :��
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_FillColor(u16 start,u16 end,RGB_TypeDef *pColor)
{
	if (start > end) //����λ��
	{
		u16 temp;
		temp = start;
		start = end;
		end = temp;
	}

	if (start >= PIXEL_NUM)
		return; //������Χ
	if (end >= PIXEL_NUM)
		end = PIXEL_NUM-1;

	//�����ɫֵ
	while(start <= end)
	{
		Copy_Color(&PixelBuf[start],pColor);
		start++;
	}
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_MovePixel
@Description:ѭ���ƶ�������ɫ
@Input      :dir:����;
@Retrun     :��
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_MovePixel(u8 dir)
{
	RGB_TypeDef temp;
	u8 i;

	if (dir) //�����ƶ�
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
	else  //�����ƶ�
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


