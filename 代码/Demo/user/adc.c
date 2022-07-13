#include "adc.h"
/*BAT_ADC--PA0、A5--PA5、A4--PA6、A3--PA7、A2--PB0、A1--PB1*/
__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,0};
__IO uint16_t adc[6]={0,0,0,0,0,0};
volatile u8 ADC_OK ;
float ADC_VBAT=0.000;

static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK1|ADC_GPIO_CLK2,ENABLE);
	
	//配置ADC的IO:PA0 PA5 PA6 PA7
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1|ADC_PIN2|ADC_PIN3|ADC_PIN4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT1,&GPIO_InitStructure);
	//PB0 PB1
	GPIO_InitStructure.GPIO_Pin = ADC_PIN5|ADC_PIN6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT2,&GPIO_InitStructure);
}

static void DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;		//DMA1_Channel1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void ADCx_Mode_config(void)
{
	DMA_InitTypeDef	DMA_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	
	//打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//打开ADC时钟
	RCC_APB2PeriphClockCmd(ADC_CLK,ENABLE);
	
	/*-----------------DMA配置-----------------*/
	//复位DMA控制器
	DMA_DeInit(ADC_DMA_CHANNEL);
	//配置DMA初始化结构体
	//外设基地址为：ADC数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	//存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	//数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	//外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//DMA传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//初始化DMA
	DMA_Init(ADC_DMA_CHANNEL,&DMA_InitStructure);
	//使能DMA通道
//	DMA_NVIC_Config();
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);	//使能DMA1通道1完成中断	
	
	DMA_Cmd(ADC_DMA_CHANNEL,ENABLE);
	
	/*-----------------ADC模式配置-----------------*/
	//ADC1时钟复位
	ADC_DeInit(ADC1);	
	//只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	//连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//软件触发模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//数据右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//通道数为6
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;
	//初始化ADC
	ADC_Init(ADCX,&ADC_InitStructure);
	//配置ADC时钟为8分频，9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//配置ADC通道转换顺序和采样时间
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL1,1,ADC_SampleTime_55Cycles5);	
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL2,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL3,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL4,4,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL5,5,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL6,6,ADC_SampleTime_55Cycles5);
	
	//使能ADC DMA请求
	ADC_DMACmd(ADCX,ENABLE);
	//开启ADC,并开始转换
	ADC_Cmd(ADCX,ENABLE);
	//初始化ADC校准寄存器
	ADC_ResetCalibration(ADCX);
	//等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCX));
	//ADC开始校准
	ADC_StartCalibration(ADCX);
	//等待校准完成
	while(ADC_GetCalibrationStatus(ADCX));
	//由于没有采用外部触发，所以使用软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADCX,ENABLE);	
}



void ADCX_config(void)
{
	ADC_GPIO_Config();
	ADCx_Mode_config();
	DMA_NVIC_Config();
}
