#include "timer.h"

u16 cnt=0;
u16 cnt2=0;
u16 cnt3=0;
u16 Speed=0;
u16 Time_OK=0;

void TIM_Configuration(TIM_TypeDef*TIMx,u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef	TIM_InitBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	if(TIMx==TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	}
	if(TIMx==TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	}
	if(TIMx==TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	}
	
//	TIM_DeInit(TIMx);		//定时器TIMx时钟复位
//	TIM_InternalClockConfig(TIMx);		//选用内部时钟即72MHZ
	/*当 APB1 的时钟分频数为 1 的 时候，TIM2~7 的时钟为 APB1 的时钟，RCC_HCLK_Div1
	而如果 APB1 的时钟分频数不为 1，那么 TIM2~7 的时 钟频率将为 APB1 时钟的两倍*/
	
	//频率计算公式：f=(psc+1)*(arr+1)/72M(Hz)
	TIM_InitBaseStructure.TIM_Period=arr-1;			//设置自动重装载值arr
	TIM_InitBaseStructure.TIM_Prescaler=psc-1;		//设置预分频值psc
	TIM_InitBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//设置时钟分割 DIV1：不分频
	TIM_InitBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式：从0计数到arr再从0开始计数
	TIM_TimeBaseInit(TIMx,&TIM_InitBaseStructure);		//初始化TIMx
	
//	TIM_ARRPreloadConfig(TIMx,ENABLE);		//使能ARR预装载缓冲器
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);			//使能定时器更新中断
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIMx,ENABLE);		//使能TIMx外设

}
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（除基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(除基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */


void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /*AHB = 72MHz,RCC_CFGR的PPRE1 = 2，所以APB1 = 36MHz,TIM2CLK = APB1*2 = 72MHz */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Time base configuration */         
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/SYSTICKFREQUENCY -1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    /* 设置更新请求源只在计数器上溢或下溢时产生中断 */
    TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Global); 
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}

/*PWM输出*/
 /**
  * @brief  PWM 配置,工作参数配置
  * @param  
  * @retval 无
  */
void TIM_PWM_Configuration(TIM_TypeDef*TIMx,u16 arr,u16 psc)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_InitBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	if(TIMx==TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	}
	if(TIMx==TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	}
	if(TIMx==TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	}
	
	//频率计算公式：f=(psc+1)*(arr+1)/72M(Hz)
	TIM_InitBaseStructure.TIM_Period=arr-1;			//设置自动重装载值arr
	TIM_InitBaseStructure.TIM_Prescaler=psc-1;		//设置预分频值psc
	TIM_InitBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//设置时钟分割 DIV1：不分频
	TIM_InitBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式：从0计数到arr再从0开始计数
	TIM_TimeBaseInit(TIMx,&TIM_InitBaseStructure);		//初始化TIMx
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//选择PWM2模式
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;			//输出比较使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性为高	
	TIM_OC1Init(TIMx,&TIM_OCInitStructure);					//初始化CH1
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);		//使能CH1预装载寄存器
		
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//选择PWM2模式
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;			//输出比较使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//输出极性为高	
	TIM_OC2Init(TIMx,&TIM_OCInitStructure);					//初始化CH2	
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);		//使能CH2预装载寄存器
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);	//使能TIMx在主ARR上的预装载寄存器
		
	TIM_Cmd(TIMx,ENABLE);
	
}

//***************************占空比调节***************************//
//占空比 = TIMx_CCRx / TIMx_ARR
//moto_l：左轮电机，moto_r：右轮电机.   数值 0-100
//***************************占空比调节***************************//
void MOTO_PWM_Out(u16 moto_l, u16 moto_r)
{
	TIM_SetCompare1(TIM4,moto_l);
	TIM_SetCompare2(TIM4,moto_r);
}


