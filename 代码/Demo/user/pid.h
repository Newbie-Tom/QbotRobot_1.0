#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "timer.h"
#include "mpu6050.h"//MPU6050������
#include "inv_mpu.h"//������������
#include "inv_mpu_dmp_motion_driver.h" //DMP��̬�����
#include "hardware.h"

typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
 
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)

}PID_LocTypeDef;

float PID_OutPut(void);

#endif
