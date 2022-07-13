#include "adc.h"
/*BAT_ADC--PA0��A5--PA5��A4--PA6��A3--PA7��A2--PB0��A1--PB1*/
__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,0};
__IO uint16_t adc[6]={0,0,0,0,0,0};
volatile u8 ADC_OK ;
float ADC_VBAT=0.000;

static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK1|ADC_GPIO_CLK2,ENABLE);
	
	//����ADC��IO:PA0 PA5 PA6 PA7
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
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;		//DMA1_Channel1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void ADCx_Mode_config(void)
{
	DMA_InitTypeDef	DMA_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	
	//��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//��ADCʱ��
	RCC_APB2PeriphClockCmd(ADC_CLK,ENABLE);
	
	/*-----------------DMA����-----------------*/
	//��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	//����DMA��ʼ���ṹ��
	//�������ַΪ��ADC���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	//�洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	//����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	//����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//�洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//�������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//�ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//DMA����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL,&DMA_InitStructure);
	//ʹ��DMAͨ��
//	DMA_NVIC_Config();
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);	//ʹ��DMA1ͨ��1����ж�	
	
	DMA_Cmd(ADC_DMA_CHANNEL,ENABLE);
	
	/*-----------------ADCģʽ����-----------------*/
	//ADC1ʱ�Ӹ�λ
	ADC_DeInit(ADC1);	
	//ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	//����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//�������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//�����Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//ͨ����Ϊ6
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;
	//��ʼ��ADC
	ADC_Init(ADCX,&ADC_InitStructure);
	//����ADCʱ��Ϊ8��Ƶ��9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����ADCͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL1,1,ADC_SampleTime_55Cycles5);	
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL2,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL3,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL4,4,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL5,5,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCX,ADC_CHANNEL6,6,ADC_SampleTime_55Cycles5);
	
	//ʹ��ADC DMA����
	ADC_DMACmd(ADCX,ENABLE);
	//����ADC,����ʼת��
	ADC_Cmd(ADCX,ENABLE);
	//��ʼ��ADCУ׼�Ĵ���
	ADC_ResetCalibration(ADCX);
	//�ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCX));
	//ADC��ʼУ׼
	ADC_StartCalibration(ADCX);
	//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCX));
	//����û�в����ⲿ����������ʹ���������ADCת��
	ADC_SoftwareStartConvCmd(ADCX,ENABLE);	
}



void ADCX_config(void)
{
	ADC_GPIO_Config();
	ADCx_Mode_config();
	DMA_NVIC_Config();
}
