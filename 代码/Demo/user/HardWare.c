#include "hardware.h"


int PWM_MAX=0,PWM_MIN=0;
int MOTO1=0,MOTO2=0;
uint8_t Fore=0,Back=0,Left=0,Right=0;
double x=0.000,y=0.000;
//***************************���õ������IO��***************************//
//˫�����
//Ӳ������˵����

//�������TB6612
//PB6---PWMA������С�������ٶ�
//PB7---PWMB������С���ҵ���ٶ�
//PB3---DIR1+��PA15---DIR1- ����С����������ת
//PB4---DIR2+��PB5---DIR2-  ����С���ҵ������ת

void Motor_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//����PB3��PB4Ϊ��ͨGPIO��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//����PA15Ϊ��ͨGPIO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}

//����
void MOTO_L(u8 state)
{
	if(state == GO)//����ǰ��
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//DIR1+=0
		GPIO_SetBits(GPIOA,GPIO_Pin_15);		//DIR1-=1
	}
	if(state == BACK)//��������
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_3);			//DIR1+=1
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);		//DIR1-=0
		
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);	//DIR1+=0
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);	//DIR1-=0
	}
				
}

//�ҵ��
void MOTO_R(u8 state)
{
	if(state == GO)//�ҵ��ǰ��
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//DIR2-=0
		GPIO_SetBits(GPIOB,GPIO_Pin_4);		//DIR2+=1
	}
	if(state == BACK)//�ҵ������
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);			//DIR2-=1
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//DIR2+=0
		
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);	//DIR2+=0
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//DIR2-=0
	}
				
}

//***************************ǰ��***************************//
//ֻҪ����INx()��״̬�Ϳ��Ըı���ת������
void car_Go(u16 moto_l, u16 moto_r)
{
	
	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM

	//���� ǰ    	//�ҵ�� ǰ
	MOTO_L(GO);       MOTO_R(GO);

//	printf("copy_go\t\n");
}

//***************************����ת***************************//
void car_Turn_Left(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM
	
	//���� ��    	//�ҵ�� ǰ
	MOTO_L(STOP);       MOTO_R(GO);

//	printf("copy_left\t\n");
}

//***************************����תȦ***************************//
void car_Left(u16 moto_l, u16 moto_r)
{

	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM

	//��ǰ��� ��    	//��ǰ��� ǰ
	MOTO_L(BACK);       MOTO_R(GO);

}

//***************************����ת***************************//
void car_Turn_Right(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM
	
	//���� ǰ    	//�ҵ�� ��
	MOTO_L(GO);       MOTO_R(STOP);

//	printf("copy_right\t\n");
}

//***************************����תȦ***************************//
void car_Right(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM
	
	//���� ǰ    	//�ҵ�� ��
	MOTO_L(GO);       MOTO_R(BACK);	
}
//***************************�����***************************//
void car_Back(u16 moto_l, u16 moto_r)
{
	MOTO_PWM_Out(moto_l, moto_r);	//˫��70%PWM

	//���� ǰ    	//�ҵ�� ǰ
	MOTO_L(BACK);       MOTO_R(BACK);

//	printf("copy_Back\t\n");
}
//***************************ͣ��***************************//
void car_Stop(void)
{
	//���� ͣ    	//�ҵ�� ͣ
	MOTO_L(STOP);       MOTO_R(STOP);

//	printf("copy_stop\t\n");
}

/*�޷�����*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*����ֵ����*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*��ֵ����*/
/*��ڲ�����PID������ɺ������PWMֵ*/
void Load(int moto1,int moto2)//moto1=-200����ת200������
{
	//1.�о������ţ���Ӧ����ת
	if(moto1>0)	//������ת
    MOTO_L(GO);//��ת
	else 		//������ת		
    MOTO_L(BACK);//��ת
	
  //1.�о������ţ���Ӧ����ת
	if(moto2>0)	//�ҵ����ת
    MOTO_R(GO);
	else 		//�ҵ����ת			
    MOTO_R(BACK);	
  //2.�о�PWMֵ
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
 //��ȡ�ַ����е�����ֵ
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

/*����KEY1 KEY2 & ������BELL*/
 /**
  * @brief  �����ͷ�������GPIO ����,������������
  * @param  ��
  * @retval ��
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



//�жϰ����Ƕ̰����ǳ���
//������timeΪ�ж�ʱ�����ã�3000Ϊ3s
//����ֵ��0Ϊû�а��£�1Ϊ�̰���2Ϊ����
u8 KeyPressed(void)
{
	if(KEY1==0)				//��ɫ��(���ü�)����ʱ	//״̬��
	{
		delay_ms(10);		//��ʱ����
		if(KEY1==0)
		{
			while(KEY1==0)		//�ȴ������ͷ�	
				delay_ms(10);
			return 3;
		}
		
	}
	
	if(KEY2==0)			//��ɫ�������ݼӣ�
	{
		delay_ms(10);		//��ʱ����
		if(KEY2==0)
		{
			while(KEY2==0)		//�ȴ������ͷ�
			
				delay_ms(10);
			return 4;
			
		}	
	}

}



 
