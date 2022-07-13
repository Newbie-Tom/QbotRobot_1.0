#include "hardware.h"


int PWM_MAX=0,PWM_MIN=0;
int MOTO1=0,MOTO2=0;
uint8_t Fore=0,Back=0,Left=0,Right=0;
double x=0.000,y=0.000;
//***************************配置电机驱动IO口***************************//
//双驱电机
//硬件连接说明：

//电机控制TB6612
//PB6---PWMA，控制小车左电机速度
//PB7---PWMB，控制小车右电机速度
//PB3---DIR1+，PA15---DIR1- 控制小车左电机正反转
//PB4---DIR2+，PB5---DIR2-  控制小车右电机正反转

void Motor_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//复用PB3、PB4为普通GPIO口	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//复用PA15为普通GPIO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}

//左电机
void MOTO_L(u8 state)
{
	if(state == GO)//左电机前进
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//DIR1+=0
		GPIO_SetBits(GPIOA,GPIO_Pin_15);		//DIR1-=1
	}
	if(state == BACK)//左电机后退
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_3);			//DIR1+=1
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);		//DIR1-=0
		
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);	//DIR1+=0
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);	//DIR1-=0
	}
				
}

//右电机
void MOTO_R(u8 state)
{
	if(state == GO)//右电机前进
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//DIR2-=0
		GPIO_SetBits(GPIOB,GPIO_Pin_4);		//DIR2+=1
	}
	if(state == BACK)//右电机后退
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);			//DIR2-=1
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//DIR2+=0
		
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);	//DIR2+=0
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//DIR2-=0
	}
				
}

//***************************前进***************************//
//只要配置INx()的状态就可以改变电机转动方向
void car_Go(u16 moto_l, u16 moto_r)
{
	
	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM

	//左电机 前    	//右电机 前
	MOTO_L(GO);       MOTO_R(GO);

//	printf("copy_go\t\n");
}

//***************************向左转***************************//
void car_Turn_Left(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM
	
	//左电机 后    	//右电机 前
	MOTO_L(STOP);       MOTO_R(GO);

//	printf("copy_left\t\n");
}

//***************************向左转圈***************************//
void car_Left(u16 moto_l, u16 moto_r)
{

	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM

	//左前电机 后    	//右前电机 前
	MOTO_L(BACK);       MOTO_R(GO);

}

//***************************向右转***************************//
void car_Turn_Right(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM
	
	//左电机 前    	//右电机 后
	MOTO_L(GO);       MOTO_R(STOP);

//	printf("copy_right\t\n");
}

//***************************向右转圈***************************//
void car_Right(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM
	
	//左电机 前    	//右电机 后
	MOTO_L(GO);       MOTO_R(BACK);	
}
//***************************向后倒退***************************//
void car_Back(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//双轮70%PWM

	//左电机 前    	//右电机 前
	MOTO_L(BACK);       MOTO_R(BACK);

//	printf("copy_Back\t\n");
}
//***************************停车***************************//
void car_Stop(void)
{
	//左电机 停    	//右电机 停
	MOTO_L(STOP);       MOTO_R(STOP);

//	printf("copy_stop\t\n");
}

/*限幅函数*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	//左电机正转
    MOTO_L(GO);//正转
	else 		//左电机反转		
    MOTO_L(BACK);//反转
	
  //1.研究正负号，对应正反转
	if(moto2>0)	//右电机正转
    MOTO_R(GO);
	else 		//右电机反转			
    MOTO_R(BACK);	
  //2.研究PWM值
	MOTO_PWM_Out(GFP_abs(moto1),GFP_abs(moto2));
}

char PWM_Zero=0,stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>60)
	{
		Load(PWM_Zero,PWM_Zero);
		stop=1;
	}
}
 //获取字符串中的坐标值
void GetLocation( char *Str,double *x,double *y)
{
	char *p =Str;
	char s[4][9];
	uint8_t i=0,m=0,n=0;	
	while(*(p+i)!='\0'){
		if(*(p+i)!=','){
			s[m][n] = *(p+i);
			n++;
		}
		else{
			s[m][n] ='\0';
			m++;
			n=0;
		}
		i++;
	}
	*( *(s+m) + n) = '\0';
	n=0;
	*x = atof(s[2]);
	while(*( *(s+3) + n)!='*'){
		*(p+n) = *( *(s+3) + n);
		n++;
	}
	*(p+n) = '\0';
	*y = atof(p);
}

/*按键KEY1 KEY2 & 蜂鸣器BELL*/
 /**
  * @brief  按键和蜂鸣器的GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */

void Key_Bell_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//KEY1:PB8 KEY2:PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//Bell:PC13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}



//判断按键是短按还是长按
//参数：time为判断时间设置，3000为3s
//返回值：0为没有按下，1为短按，2为长按
u8 KeyPressed(void)
{
	if(KEY1==0)				//红色键(设置键)按下时	//状态键
	{
		delay_ms(10);		//延时消抖
		if(KEY1==0)
		{
			while(KEY1==0)		//等待按键释放	
				delay_ms(10);
			return 3;
		}
		
	}
	
	if(KEY2==0)			//蓝色按键（递加）
	{
		delay_ms(10);		//延时消抖
		if(KEY2==0)
		{
			while(KEY2==0)		//等待按键释放
			
				delay_ms(10);
			return 4;
			
		}	
	}

}



 
