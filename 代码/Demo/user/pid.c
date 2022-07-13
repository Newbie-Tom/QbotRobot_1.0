#include "pid.h"


float Kp=0.5;
float Ki=0.0015;
float Kd=0.3;

//从左到右为A5 A4 A3 A2 A1
//右偏位置差为负
//左偏位置差为正
float PID_OutPut(void)
{
	
	float error=0,last_error=0;    //此次和上次的误差      
	static float P_term,D_term,I_term;    //积分累加项

	float output;      //PID输出
	
	error = error+2*ADC_ConvertedValue[5];		//A1 右2
	error = error+1*ADC_ConvertedValue[4];		//A2 右1
	error = error-1*ADC_ConvertedValue[2];		//A4 左1
	error = error-2*ADC_ConvertedValue[1];		//A5 左2
	
	P_term = Kp*error;		//比例值
	D_term = Kd*(error-last_error);	//微分值
	I_term = Ki*(I_term+error);		//积分值
	output = P_term+I_term+D_term;
	
	last_error = error;
	return output;

}
