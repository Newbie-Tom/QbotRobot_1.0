#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "timer.h"
#include "mpu6050.h"//MPU6050驱动库
#include "inv_mpu.h"//陀螺仪驱动库
#include "inv_mpu_dmp_motion_driver.h" //DMP姿态解读库
#include "hardware.h"

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative
 
  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)

}PID_LocTypeDef;

float PID_OutPut(void);

#endif
