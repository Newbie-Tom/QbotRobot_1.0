#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "oled.h"
#include "bmp.h"
#include "adc.h"
#include "timer.h"
#include "hardware.h"
#include "rgb.h"
#include "mpu6050.h"//MPU6050驱动库
#include "niming.h"//模拟四轴数据库
#include "inv_mpu.h"//陀螺仪驱动库
#include "inv_mpu_dmp_motion_driver.h" //DMP姿态解读库
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pid.h"

extern volatile u8 trace_OK;
extern volatile u8 MPU_OK;
extern float ADC_VBAT;
extern float pitch,roll,yaw; 		//欧拉角:俯仰角，偏航角，滚转角
extern short aacx,aacy,aacz;		//加速度传感器原始数据  angular acceleration
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据  gyroscope
extern short temp;					//温度

char tmp_buf[33];			//字符串数组
float pitch,roll,yaw; 		//欧拉角:俯仰角，偏航角，滚转角
short aacx,aacy,aacz;		//加速度传感器原始数据  angular acceleration
short gyrox,gyroy,gyroz;	//陀螺仪原始数据  gyroscope
short temp;					//温度

struct MPU6050				//MPU6050结构体
{
	u8 flag;				//采集成功标志位
	u8 speed;				//上报速度
}mpu6050;					//唯一结构体变量

int main(void);
void MPU_Read(void);
void DATA_Report(void);


#endif 


