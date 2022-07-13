#include "pid.h"


float Kp=0.5;
float Ki=0.0015;
float Kd=0.3;

//������ΪA5 A4 A3 A2 A1
//��ƫλ�ò�Ϊ��
//��ƫλ�ò�Ϊ��
float PID_OutPut(void)
{
	
	float error=0,last_error=0;    //�˴κ��ϴε����      
	static float P_term,D_term,I_term;    //�����ۼ���

	float output;      //PID���
	
	error = error+2*ADC_ConvertedValue[5];		//A1 ��2
	error = error+1*ADC_ConvertedValue[4];		//A2 ��1
	error = error-1*ADC_ConvertedValue[2];		//A4 ��1
	error = error-2*ADC_ConvertedValue[1];		//A5 ��2
	
	P_term = Kp*error;		//����ֵ
	D_term = Kd*(error-last_error);	//΢��ֵ
	I_term = Ki*(I_term+error);		//����ֵ
	output = P_term+I_term+D_term;
	
	last_error = error;
	return output;

}
