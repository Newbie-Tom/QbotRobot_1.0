#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include <math.h>
#include <stdlib.h>
#define GO    0//定义电机状态 前进
#define BACK  1//后退
#define STOP  2//停车

#define Bell		PCout(13)
#define KEY1		PBin(8)
#define KEY2		PBin(9)

//#define Dir1  	PBout(3)
//#define Dir1_  	PAout(15)

//#define Dir2  	PBout(4)
//#define Dir2_  	PBout(5)

extern int PWM_MAX,PWM_MIN;
extern int MOTO1,MOTO2;
extern uint8_t Fore,Back,Left,Right;
extern double x,y;
void Motor_Config(void);
void MOTO_L(u8 state);
void MOTO_R(u8 state);
extern void car_Go(u16 moto_l, u16 moto_r);
void car_Turn_Left(u16 moto_l, u16 moto_r);
void car_Left(u16 moto_l, u16 moto_r);
void car_Turn_Right(u16 moto_l, u16 moto_r);
void car_Right(u16 moto_l, u16 moto_r);
void car_Back(u16 moto_l, u16 moto_r);
void car_Stop(void);
void Key_Bell_Init(void);
u8 KeyPressed(void);
void GetLocation( char *Str,double *x,double *y);
void Limit(int *motoA,int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);


#endif

