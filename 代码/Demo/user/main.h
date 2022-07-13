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
#include "mpu6050.h"//MPU6050������
#include "niming.h"//ģ���������ݿ�
#include "inv_mpu.h"//������������
#include "inv_mpu_dmp_motion_driver.h" //DMP��̬�����
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pid.h"

extern volatile u8 trace_OK;
extern volatile u8 MPU_OK;
extern float ADC_VBAT;
extern float pitch,roll,yaw; 		//ŷ����:�����ǣ�ƫ���ǣ���ת��
extern short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����  angular acceleration
extern short gyrox,gyroy,gyroz;	//������ԭʼ����  gyroscope
extern short temp;					//�¶�

char tmp_buf[33];			//�ַ�������
float pitch,roll,yaw; 		//ŷ����:�����ǣ�ƫ���ǣ���ת��
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����  angular acceleration
short gyrox,gyroy,gyroz;	//������ԭʼ����  gyroscope
short temp;					//�¶�

struct MPU6050				//MPU6050�ṹ��
{
	u8 flag;				//�ɼ��ɹ���־λ
	u8 speed;				//�ϱ��ٶ�
}mpu6050;					//Ψһ�ṹ�����

int main(void);
void MPU_Read(void);
void DATA_Report(void);


#endif 


