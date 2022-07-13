#ifndef __TIMER_H
#define __TIMER_H

#define SYSTICKPERIOD                    0.000001
#define SYSTICKFREQUENCY            (1/SYSTICKPERIOD)
#include "stm32f10x.h"
#include "sys.h"


extern u16 cnt;
extern u16 cnt2;
extern u16 cnt3;
extern u16 Speed;
extern u16 Time_OK;

void TIM_Configuration(TIM_TypeDef*TIMx,u16 arr,u16 psc);
void TIM3_Init(void);
void TIM_NVIC_Configuration(u8 Num);
void TIM_PWM_Configuration(TIM_TypeDef*TIMx,u16 arr,u16 psc);
void MOTO_PWM_Out(u16 moto_l, u16 moto_r);

#endif
