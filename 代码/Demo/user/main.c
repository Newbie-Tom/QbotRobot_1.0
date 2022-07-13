#include "main.h"

volatile u8 trace_OK;
volatile u8 MPU_OK;
int main(void)
{

	uint8_t Key_Value=0,len=0,t;
	uint8_t remote_flag =0,trace_flag=0,weight_flag=0;
	uint16_t Temp_x =0,Temp_y=0;
	double x=0,y=0;

	RGB_TypeDef RGB_temp,RGB_temp1;
	uint8_t BLE_RX[64];//���������������ݻ�����
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	USART_Config(9600);	//����3��ʼ��Ϊ������115200
	//time = (arr-1)*(psc-1)/72M	TIM3��ʱ1ms
	TIM_Configuration(TIM3,1000,72);
	TIM_PWM_Configuration(TIM4,1000,2	);	//f = 72M/(arr-1)*(psc-1) ���Ƶ��20KHz
	ADCX_config();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	Motor_Config();
	Key_Bell_Init();
	RGB_Init();
	OLED_ShowString(48,0,"QBOT");
	OLED_ShowString(16,2,"Initializing");
	OLED_ShowString(48,4,"...");
	OLED_ShowString(8,6,"MPU:");
	OLED_ShowString(48,6,"preparing");
	MPU_Init();
	while(mpu_dmp_init())                            //��ʼ��mpu_dmp��
 	{
		Bell =! Bell;
		delay_ms(50);
		Bell =! Bell;
		printf("Initialization failed��\r\n");		//���ڳ�ʼ��ʧ���ϱ�
		OLED_ShowString(48,6,"failed!  ");
	}
	printf("Initialization succeed��\r\n");			//���ڳ�ʼ���ɹ��ϱ�
	OLED_ShowString(48,6,"succeed! ");
	delay_ms(999);									//��ʱ��������ʾ
//	uart_init(500000);
	ADC_OK =0;
	trace_OK = 0;
	MPU_OK =0;
	RGB_temp.R = 0x00;
	RGB_temp.G = 0x00;
	RGB_temp.B = 0xff;
	RGB_temp1.R =0x00;
	RGB_temp1.G = 0xff;
	RGB_temp1.B = 0x00;
	WS2812B_FillColor(0,0,&RGB_temp1);
	WS2812B_FillColor(1,1,&RGB_temp);
//	mpu6050.flag = 0;                               //�ɼ��ɹ���־λ��ʼ��
//	mpu6050.speed = 0;								//�ϱ��ٶȳ�ʼ��
	delay_ms(2000);
	//��ʼ�����
	OLED_Clear();		
	OLED_ShowString(48,0,"QBOT");
	OLED_ShowString(0,2,"pitch:");
	OLED_ShowNum(56,2,0,2,16);
	OLED_ShowChar(72,2,'.');
	OLED_ShowNum(80,2,0,1,16);
	OLED_ShowString(0,4,"roll:");
	OLED_ShowNum(48,4,0,2,16);
	OLED_ShowChar(64,4,'.');
	OLED_ShowNum(72,4,0,1,16);
	OLED_ShowString(0,6,"yaw:");
	OLED_ShowNum(40,6,0,2,16);
	OLED_ShowChar(56,6,'.');
	OLED_ShowNum(64,6,0,1,16);
	
	while(1) 
	{							
//		MPU_Read();    //MPU6050���ݶ�ȡ
//		DATA_Report(); //MPU6050�����ϱ�
		if(ADC_OK){
			ADC_VBAT = ADC_ConvertedValue[0]*3.3/4096;
			if(ADC_VBAT>=1.4&&ADC_VBAT<=2.1)
//				printf("��ʱ��ص�����Ϊ%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
			{
				if(1.4<=ADC_VBAT &&ADC_VBAT<=1.575)
					OLED_DrawBMP(110,0,126,1,Battery25_BMP);
				else 
					if(1.575<ADC_VBAT &&ADC_VBAT<=1.75)
						OLED_DrawBMP(110,0,126,1,Battery50_BMP);
				else
					if(1.75<ADC_VBAT &&ADC_VBAT<=1.925)
						OLED_DrawBMP(110,0,126,1,Battery75_BMP);
				else
					if(1.925<ADC_VBAT &&ADC_VBAT<=2.1)
						OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
			}	
			else
				if(ADC_VBAT<1.4)
//					printf("��ʱ��ص�����Ϊ%d%%\r\n",0);
					OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
			else
				if(ADC_VBAT>2.1)
//					printf("��ʱ��ص�����Ϊ%d%%\r\n",100);		
					OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
		}
		if(MPU_OK)
		{
			MPU_OK =0;
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp����õ����ݣ��Է���ֵ�����ж�
			{ 
				temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
				if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
				{
					temp=-temp;							//�Ը�����ȡ��
				}
				else                                    //�ж�Ϊ��ʱ
				{
				}

				temp=pitch*10;							 //��tempΪpitch
				if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
				{
					temp=-temp;						    //�Ը�����ȡ��		
				}
				else                                    //�ж�Ϊ��ʱ 
				{
				}
//							printf(" pitch:%d.%d",temp/10,temp%10); //ͨ������1���pitch	
				OLED_ShowNum(56,2,temp/10,2,16);
				OLED_ShowChar(72,2,'.');
				OLED_ShowNum(80,2,temp%10,1,16);
				temp=roll*10;                            //��tempΪroll
				if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
				{
					temp=-temp;						    //�Ը�����ȡ��	
				}
				else                                    //�ж�Ϊ��ʱ
				{
				}
//							printf(" roll:%d.%d ",temp/10,temp%10);//ͨ������1���roll
				OLED_ShowNum(48,4,temp/10,2,16);
				OLED_ShowChar(64,4,'.');
				OLED_ShowNum(72,4,temp%10,1,16);
				temp=yaw*10;                           //��tempΪyaw
				if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
				{
					temp=-temp;						    //�Ը�����ȡ��
				}
				else                                    //�ж�Ϊ��ʱ
				{
				}
//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//ͨ������1���yaw	
				OLED_ShowNum(40,6,temp/10,2,16);
				OLED_ShowChar(56,6,'.');
				OLED_ShowNum(64,6,temp%10,1,16);
			}
		}
		Key_Value = KeyPressed();
		switch(Key_Value)
		{
			case 3://ң�غ�������Ӧģʽ
					car_Stop();
					OLED_Clear();		
					OLED_ShowString(48,0,"QBOT");
					OLED_ShowString(0,2,"pitch:");
					OLED_ShowNum(56,2,0,2,16);
					OLED_ShowChar(72,2,'.');
					OLED_ShowNum(80,2,0,1,16);
					OLED_ShowString(0,4,"roll:");
					OLED_ShowNum(48,4,0,2,16);
					OLED_ShowChar(64,4,'.');
					OLED_ShowNum(72,4,0,1,16);
					OLED_ShowString(0,6,"yaw:");
					OLED_ShowNum(40,6,0,2,16);
					OLED_ShowChar(56,6,'.');
					OLED_ShowNum(64,6,0,1,16);
					while(KEY2!=0){
//							printf("this is key1 Demo\r\n");
						//		USART3_Receive(BLE_RX, 23);
//		printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//		for(t=0;t<rx3_count;t++)
//		{
//			USART_SendData(USART1, BLE_RX[t]);//�򴮿�1��������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//		}
//		printf("\r\n\r\n");//���뻻��'
//		printf("���ĸ��ǣ�%X���������%X",BLE_RX[3],BLE_RX[4]);
		
//		if((BLE_RX[3] == 0xB2)&&(BLE_RX[4]==0xB6))//ǰ��
//		{
//			car_Go(65,65);
//		}
//		else if((BLE_RX[3] == 0xB4)&&(BLE_RX[4]==0xB8))//����ת
//		{
//			car_Turn_Left(60,60);
//			delay_ms(200);//���޽Ƕȴ������������ڳ��ٲ�ͬ��������ʱ����ʵ�ִ�ŵ�ת�� 90�ȡ�
//			BLE_RX[3] = 0x00;//�建��
//		}
//		else if((BLE_RX[3] == 0xB7)&&(BLE_RX[4]==0xBB))//��תȦ
//		{
////					car_Left(60,60,60,60);
////					delay_ms(200);//���޽Ƕȴ������������ڳ��ٲ�ͬ��������ʱ����ʵ�ִ�ŵ�ת�� 90�ȡ�
////					BLE_RX[3] = 0x00;//�建��
//		}
//		
//		else if((BLE_RX[3] == 0xB5)&&(BLE_RX[4]==0xB9))//ͣ��
//		{
//			car_Stop();
//		}
//		else if((BLE_RX[3] == 0xB6)&&(BLE_RX[4]==0xBA))//����ת
//		{
//			car_Turn_Right(60,60);
//			delay_ms(200);//���޽Ƕȴ������������ڳ��ٲ�ͬ��������ʱ����ʵ�ִ�ŵ�ת�� 90�ȡ�
//			BLE_RX[3] = 0x00;//�建��
//		}
//		else if((BLE_RX[3] == 0xB9)&&(BLE_RX[4]==0xBD))//��תȦ
//		{
////					car_Right(60,60,60,60);
////					delay_ms(200);//���޽Ƕȴ������������ڳ��ٲ�ͬ��������ʱ����ʵ�ִ�ŵ�ת�� 90�ȡ�
////					BLE_RX[3] = 0x00;//�建��
//		}
//		else if((BLE_RX[3] == 0xB8)&&(BLE_RX[4]==0xBC))//����
//		{
//			car_Back(60,60);
//		} 
				
//		delay_ms(500);
						OLED_DrawBMP(0,0,16,1,Signal);		//�ź�ͼ��
						if(ADC_OK){
							ADC_OK=0;
							ADC_VBAT = ADC_ConvertedValue[0]*3.3/4096;
							if(ADC_VBAT>=1.4&&ADC_VBAT<=2.1)
				//				printf("��ʱ��ص�����Ϊ%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
							{
								if(1.4<=ADC_VBAT &&ADC_VBAT<=1.575)
									OLED_DrawBMP(110,0,126,1,Battery25_BMP);
								else 
									if(1.575<ADC_VBAT &&ADC_VBAT<=1.75)
										OLED_DrawBMP(110,0,126,1,Battery50_BMP);
								else
									if(1.75<ADC_VBAT &&ADC_VBAT<=1.925)
										OLED_DrawBMP(110,0,126,1,Battery75_BMP);
								else
									if(1.925<ADC_VBAT &&ADC_VBAT<=2.1)
										OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
							}	
							else
								if(ADC_VBAT<1.4)
				//					printf("��ʱ��ص�����Ϊ%d%%\r\n",0);
									OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
							else
								if(ADC_VBAT>2.1)
				//					printf("��ʱ��ص�����Ϊ%d%%\r\n",100);		
									OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
						}
						if(MPU_OK)
						{
							MPU_OK =0;
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp����õ����ݣ��Է���ֵ�����ж�
							{ 
								temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
								MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
								MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;							//�Ը�����ȡ��
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}

								temp=pitch*10;							 //��tempΪpitch
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��		
								}
								else                                    //�ж�Ϊ��ʱ 
								{
								}
	//							printf(" pitch:%d.%d",temp/10,temp%10); //ͨ������1���pitch	
								OLED_ShowNum(56,2,temp/10,2,16);
								OLED_ShowChar(72,2,'.');
								OLED_ShowNum(80,2,temp%10,1,16);
								temp=roll*10;                            //��tempΪroll
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��	
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}
	//							printf(" roll:%d.%d ",temp/10,temp%10);//ͨ������1���roll
								OLED_ShowNum(48,4,temp/10,2,16);
								OLED_ShowChar(64,4,'.');
								OLED_ShowNum(72,4,temp%10,1,16);
								temp=yaw*10;                           //��tempΪyaw
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}
	//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//ͨ������1���yaw	
								OLED_ShowNum(40,6,temp/10,2,16);
								OLED_ShowChar(56,6,'.');
								OLED_ShowNum(64,6,temp%10,1,16);
							}
						}
							if(USART3_RX_STA&0x8000)
							{					   
								len=USART3_RX_Count;//�õ��˴ν��յ������ݳ���
								printf("\r\n�����͵���ϢΪ:\r\n\r\n");
								for(t=0;t<len;t++)
								{
									USART_SendData(USART1, USART3_RX_BUF[t]);//�򴮿�1��������
									while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
								}
								printf("\r\n\r\n");//���뻻��'
							if(strstr((char*)USART3_RX_BUF,"CM")!=NULL)		//������Ӧģʽ
							{
								weight_flag =1;
								remote_flag =0;
							}
							else
							{
								weight_flag =0;		
							}
							if(strstr((char*)USART3_RX_BUF,"CJ")!=NULL)		//ҡ�˿���ģʽ
							{	
								remote_flag =1;
								weight_flag =0;
							}
								else
								{
									remote_flag =0;
								}
								if(strstr((char*)USART3_RX_BUF,"CS")!=NULL){	
									car_Stop();
									remote_flag =0;
									weight_flag =0;
								}
								if(remote_flag)			//ҡ��ģʽ��
								{
									GetLocation((char*)USART3_RX_BUF,&x,&y);
									printf("x=%.3f,y=%.3f\r\n",x,y);//���뻻��
									if((float)x==0&&(float)y != 0)	{
										if((float)y>0){
											Temp_y = GFP_abs((int)((float)y*1000));
											car_Go(Temp_y,Temp_y);	//ȫ��ǰ��
										}
										else if((float)y<0)
										{
											Temp_y = GFP_abs((int)((float)y*1000));
											car_Back(Temp_y,Temp_y);	//ȫ�ٺ���
										}
									}		
								 if (((float)x > 0 && (float)y < 0) || ((float)x > 0 && (float)y > 0)) { //��һ�������޷���  ��ת������  ����>����
									if((float)y > 0)	//��һ����
									{
										if((float)x>=(float)y){
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										car_Right(Temp_x,Temp_y);
									}
									else if((float)x<(float)y){
										
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										if(Temp_x<100)
										{
											Temp_x =Temp_y;
											car_Go(Temp_x,Temp_y);
										}
										else	
											car_Right(Temp_y,Temp_x);
									}
								}
									else if((float)y < 0)	//��������
									{
										if(GFP_abs((float)x)>=GFP_abs((float)y)){
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										car_Right(Temp_x,Temp_y);
									}
									else if(GFP_abs((float)x)<GFP_abs((float)y)){
										
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										if(Temp_x<100)
										{
											Temp_x =Temp_y;
											car_Back(Temp_x,Temp_y);
										}
										else	
											car_Right(Temp_y,Temp_x);
									}
									}
								} else if (((float)x < 0 && (float)y  < 0) || ((float)x < 0 && (float)y > 0)) {//�ڶ��������޷��� ��ת������  ����>����
									if((float)y > 0)	//�ڶ�����
									{
										if((float)x>=(float)y){
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										car_Left(Temp_y,Temp_x);
									}
									else if((float)x<(float)y){
										
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										if(Temp_x<100)
										{
											Temp_x =Temp_y;
											car_Go(Temp_x,Temp_y);
										}
										else	
											car_Left(Temp_x,Temp_y);
									}
								}
									else if((float)y < 0)	//��������
									{
										if(GFP_abs((float)x)>=GFP_abs((float)y)){
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										car_Left(Temp_y,Temp_x);
									}
									else if(GFP_abs((float)x)<GFP_abs((float)y)){
										
										Temp_x = GFP_abs((int)((float)x*1000));	
										Temp_y = GFP_abs((int)((float)y*1000));
										if(Temp_x<100)
										{
											Temp_x =Temp_y;
											car_Back(Temp_x,Temp_y);
										}
										else	
											car_Left(Temp_x,Temp_y);
									}
									}
								}
								if ((float)x == 0 && (float)y == 0) {
									car_Stop();		//С��ֹͣ
								}
							}
							if(weight_flag)		//������Ӧģʽ	x:ǰ��	y:����
							{
								GetLocation((char*)USART3_RX_BUF,&x,&y);
								printf("x=%.3f,y=%.3f\r\n",x,y);//���뻻��
								if((float)y==0&&(float)x!= 0)	{
									if((float)x>0){
										Temp_x = GFP_abs((int)((float)x*10));
										car_Go(Temp_x,Temp_x);	//ȫ��ǰ��
									}
									else if((float)x<0)
									{
										Temp_x = GFP_abs((int)((float)x*10));
										car_Back(Temp_x,Temp_x);	//ȫ�ٺ���
									}
								}		
								 if (((float)y > 0 && (float)x < 0) || ((float)y > 0 && (float)x > 0)) { //��һ�������޷���  ��ת������  ����>����
									if((float)x > 0)	//��һ����
									{
										if((float)y>=(float)x){	//��������
										Temp_x = GFP_abs((int)((float)x*10));	//ǰ�����
										Temp_y = GFP_abs((int)((float)y*10));	//�������
										car_Right(Temp_y,Temp_x);
									}
									else if((float)y<(float)x){	//��������
										
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										if(Temp_y<450)
										{
											Temp_y =Temp_x;
											car_Go(Temp_x,Temp_y);
										}
										else	
											car_Right(Temp_x,Temp_y);
									}
								}
									else if((float)x < 0)	//��������
									{
										if(GFP_abs((float)y)>=GFP_abs((float)x)){
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										car_Right(Temp_y,Temp_x);
									}
									else if(GFP_abs((float)y)<GFP_abs((float)x)){
										
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										if(Temp_y<450)
										{
											Temp_y =Temp_x;
											car_Back(Temp_y,Temp_x);
										}
										else	
											car_Right(Temp_x,Temp_y);
									}
									}
								} else if (((float)y < 0 && (float)x  < 0) || ((float)y < 0 && (float)x > 0)) {//�ڶ��������޷��� ��ת������  ����>����
									if((float)x > 0)	//�ڶ�����
									{
										if((float)y>=(float)x){
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										car_Left(Temp_x,Temp_y);
									}
									else if((float)y<(float)x){
										
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										if(Temp_y<450)
										{
											Temp_y =Temp_x;
											car_Go(Temp_y,Temp_x);
										}
										else	
											car_Left(Temp_y,Temp_x);
									}
								}
									else if((float)x < 0)	//��������
									{
										if(GFP_abs((float)y)>=GFP_abs((float)x)){
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										car_Left(Temp_x,Temp_y);
									}
									else if(GFP_abs((float)y)<GFP_abs((float)x)){
										
										Temp_x = GFP_abs((int)((float)x*10));	
										Temp_y = GFP_abs((int)((float)y*10));
										if(Temp_y<450)
										{
											Temp_y =Temp_x;
											car_Back(Temp_y,Temp_x);
										}
										else	
											car_Left(Temp_y,Temp_x);
									}
									}
								}
								if ((float)x == 0 && (float)y == 0) {
									car_Stop();		//С��ֹͣ
								}
							}									
								USART3_RX_STA=0;
								memset((char*)USART3_RX_BUF, 0, sizeof(USART3_RX_BUF)); 
							}
					}
					break;
			case 4:	//PIDѲ��ģʽ
					car_Stop();
					OLED_Clear();		
					OLED_ShowString(48,0,"QBOT");
					OLED_ShowString(0,2,"pitch:");
					OLED_ShowNum(56,2,0,2,16);
					OLED_ShowChar(72,2,'.');
					OLED_ShowNum(80,2,0,1,16);
					OLED_ShowString(0,4,"roll:");
					OLED_ShowNum(48,4,0,2,16);
					OLED_ShowChar(64,4,'.');
					OLED_ShowNum(72,4,0,1,16);
					OLED_ShowString(0,6,"yaw:");
					OLED_ShowNum(40,6,0,2,16);
					OLED_ShowChar(56,6,'.');
					OLED_ShowNum(64,6,0,1,16);
					while(KEY1!=0){
//							printf("this is key2 Demo\r\n");
						if(ADC_OK){
							ADC_OK=0;
							ADC_VBAT = ADC_ConvertedValue[0]*3.3/4096;
							if(ADC_VBAT>=1.4&&ADC_VBAT<=2.1)
				//				printf("��ʱ��ص�����Ϊ%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
							{
								if(1.4<=ADC_VBAT &&ADC_VBAT<=1.575)
									OLED_DrawBMP(110,0,126,1,Battery25_BMP);
								else 
									if(1.575<ADC_VBAT &&ADC_VBAT<=1.75)
										OLED_DrawBMP(110,0,126,1,Battery50_BMP);
								else
									if(1.75<ADC_VBAT &&ADC_VBAT<=1.925)
										OLED_DrawBMP(110,0,126,1,Battery75_BMP);
								else
									if(1.925<ADC_VBAT &&ADC_VBAT<=2.1)
										OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
							}	
							else
								if(ADC_VBAT<1.4)
				//					printf("��ʱ��ص�����Ϊ%d%%\r\n",0);
									OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
							else
								if(ADC_VBAT>2.1)
				//					printf("��ʱ��ص�����Ϊ%d%%\r\n",100);		
									OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
						}
						if(MPU_OK)
						{
							MPU_OK =0;
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp����õ����ݣ��Է���ֵ�����ж�
							{ 
								temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
								MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
								MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;							//�Ը�����ȡ��
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}

								temp=pitch*10;							 //��tempΪpitch
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��		
								}
								else                                    //�ж�Ϊ��ʱ 
								{
								}
	//							printf(" pitch:%d.%d",temp/10,temp%10); //ͨ������1���pitch	
								OLED_ShowNum(56,2,temp/10,2,16);
								OLED_ShowChar(72,2,'.');
								OLED_ShowNum(80,2,temp%10,1,16);
								temp=roll*10;                            //��tempΪroll
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��	
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}
	//							printf(" roll:%d.%d ",temp/10,temp%10);//ͨ������1���roll
								OLED_ShowNum(48,4,temp/10,2,16);
								OLED_ShowChar(64,4,'.');
								OLED_ShowNum(72,4,temp%10,1,16);
								temp=yaw*10;                           //��tempΪyaw
								if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
								{
									temp=-temp;						    //�Ը�����ȡ��
								}
								else                                    //�ж�Ϊ��ʱ
								{
								}
	//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//ͨ������1���yaw	
								OLED_ShowNum(40,6,temp/10,2,16);
								OLED_ShowChar(56,6,'.');								
								OLED_ShowNum(64,6,temp%10,1,16);
							}
					}
						if(trace_OK)
						{
							trace_OK =0;
							car_Go(500+PID_OutPut(),500-PID_OutPut());
						}
					}
					break;
			default:
					break;
			
		}
//		RGB_LED_Green();
		delay_ms(100);
//		WS2812B_MovePixel(1);

//		WS2812B_RefreshPixel();
		
	}
}
/**
  * @brief  MPU6050���ݶ�ȡ����
  * @param  ��
  * @retval ��
  */
void MPU_Read(void)
{
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp����õ����ݣ��Է���ֵ�����ж�
	{ 
		temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
		mpu6050.speed++;                            //�ϱ��ٶ��Լ�
		if(mpu6050.speed == 1)						//�ϱ��ٶ���ֵ����
		{
			mpu6050.flag = 1;						//�ɼ��ɹ���־λ����Ϊ��Ч
			mpu6050.speed = 0;						//�ϱ��ٶȹ���
		}	
	}
	else 											//�ɼ����ɹ�										
	{
		mpu6050.flag = 0;							//�ɼ��ɹ���־λ����Ϊ��Ч
	}	
}
/**
  * @brief  MPU6050�����ϱ�
  * @param  ��
  * @retval ��
  */
void DATA_Report(void)
{
	if(mpu6050.flag == 1)									//�ɼ��ɹ�ʱ
	{
		mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���ͼ��ٶȴ��������ݺ�����������
		usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));//�ϱ���������̬����
		mpu6050.flag = 0;									//�ɼ��ɹ���־λ����Ϊ��Ч
	}
	else ;														//������
}
