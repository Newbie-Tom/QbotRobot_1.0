#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"



#define ADCX					ADC1
#define ADC_APBxClock_FUN		RCC_APB2PeriphClockCmd
#define ADC_CLK					RCC_APB2Periph_ADC1

#define ADC_GPIO_APBxClock_FUN	RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK1			RCC_APB2Periph_GPIOA
#define ADC_PORT1				GPIOA

#define ADC_GPIO_CLK2			RCC_APB2Periph_GPIOB
#define ADC_PORT2				GPIOB

//转换通道数
#define NOFCHANEL				6

#define ADC_PIN1				GPIO_Pin_0
#define ADC_CHANNEL1			ADC_Channel_0

#define ADC_PIN2				GPIO_Pin_5
#define ADC_CHANNEL2			ADC_Channel_5

#define ADC_PIN3				GPIO_Pin_6
#define ADC_CHANNEL3			ADC_Channel_6

#define ADC_PIN4				GPIO_Pin_7
#define ADC_CHANNEL4			ADC_Channel_7

#define ADC_PIN5				GPIO_Pin_0
#define ADC_CHANNEL5			ADC_Channel_8

#define ADC_PIN6				GPIO_Pin_1
#define ADC_CHANNEL6			ADC_Channel_9

#define ADC_DMA_CHANNEL			DMA1_Channel1

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
extern __IO uint16_t adc[6];
extern volatile u8 ADC_OK ;
extern float ADC_VBAT;

void ADCX_config(void);
#endif
