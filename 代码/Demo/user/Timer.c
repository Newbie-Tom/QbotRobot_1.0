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
	
//	TIM_DeInit(TIMx);		//��ʱ��TIMxʱ�Ӹ�λ
//	TIM_InternalClockConfig(TIMx);		//ѡ���ڲ�ʱ�Ӽ�72MHZ
	/*�� APB1 ��ʱ�ӷ�Ƶ��Ϊ 1 �� ʱ��TIM2~7 ��ʱ��Ϊ APB1 ��ʱ�ӣ�RCC_HCLK_Div1
	����� APB1 ��ʱ�ӷ�Ƶ����Ϊ 1����ô TIM2~7 ��ʱ ��Ƶ�ʽ�Ϊ APB1 ʱ�ӵ�����*/
	
	//Ƶ�ʼ��㹫ʽ��f=(psc+1)*(arr+1)/72M(Hz)
	TIM_InitBaseStructure.TIM_Period=arr-1;			//�����Զ���װ��ֵarr
	TIM_InitBaseStructure.TIM_Prescaler=psc-1;		//����Ԥ��Ƶֵpsc
	TIM_InitBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//����ʱ�ӷָ� DIV1������Ƶ
	TIM_InitBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ����0������arr�ٴ�0��ʼ����
	TIM_TimeBaseInit(TIMx,&TIM_InitBaseStructure);		//��ʼ��TIMx
	
//	TIM_ARRPreloadConfig(TIMx,ENABLE);		//ʹ��ARRԤװ�ػ�����
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);			//ʹ�ܶ�ʱ�������ж�
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIMx,ENABLE);		//ʹ��TIMx����

}
/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У���������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(��������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */


void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /*AHB = 72MHz,RCC_CFGR��PPRE1 = 2������APB1 = 36MHz,TIM2CLK = APB1*2 = 72MHz */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Time base configuration */         
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/SYSTICKFREQUENCY -1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    /* ���ø�������Դֻ�ڼ��������������ʱ�����ж� */
    TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Global); 
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}

/*PWM���*/
 /**
  * @brief  PWM ����,������������
  * @param  
  * @retval ��
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
	
	//Ƶ�ʼ��㹫ʽ��f=(psc+1)*(arr+1)/72M(Hz)
	TIM_InitBaseStructure.TIM_Period=arr-1;			//�����Զ���װ��ֵarr
	TIM_InitBaseStructure.TIM_Prescaler=psc-1;		//����Ԥ��Ƶֵpsc
	TIM_InitBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//����ʱ�ӷָ� DIV1������Ƶ
	TIM_InitBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ����0������arr�ٴ�0��ʼ����
	TIM_TimeBaseInit(TIMx,&TIM_InitBaseStructure);		//��ʼ��TIMx
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//ѡ��PWM2ģʽ
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;			//����Ƚ�ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������Ϊ��	
	TIM_OC1Init(TIMx,&TIM_OCInitStructure);					//��ʼ��CH1
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);		//ʹ��CH1Ԥװ�ؼĴ���
		
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//ѡ��PWM2ģʽ
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;			//����Ƚ�ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		//�������Ϊ��	
	TIM_OC2Init(TIMx,&TIM_OCInitStructure);					//��ʼ��CH2	
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);		//ʹ��CH2Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);	//ʹ��TIMx����ARR�ϵ�Ԥװ�ؼĴ���
		
	TIM_Cmd(TIMx,ENABLE);
	
}

//***************************ռ�ձȵ���***************************//
//ռ�ձ� = TIMx_CCRx / TIMx_ARR
//moto_l�����ֵ����moto_r�����ֵ��.   ��ֵ 0-100
//***************************ռ�ձȵ���***************************//
void MOTO_PWM_Out(u16 moto_l, u16 moto_r)
{
	TIM_SetCompare1(TIM4,moto_l);
	TIM_SetCompare2(TIM4,moto_r);
}


