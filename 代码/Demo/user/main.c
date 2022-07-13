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
	uint8_t BLE_RX[64];//定义蓝牙接收数据缓存区
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	USART_Config(9600);	//串口3初始化为波特率115200
	//time = (arr-1)*(psc-1)/72M	TIM3定时1ms
	TIM_Configuration(TIM3,1000,72);
	TIM_PWM_Configuration(TIM4,1000,2	);	//f = 72M/(arr-1)*(psc-1) 电机频率20KHz
	ADCX_config();
	OLED_Init();			//初始化OLED  
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
	while(mpu_dmp_init())                            //初始化mpu_dmp库
 	{
		Bell =! Bell;
		delay_ms(50);
		Bell =! Bell;
		printf("Initialization failed！\r\n");		//串口初始化失败上报
		OLED_ShowString(48,6,"failed!  ");
	}
	printf("Initialization succeed！\r\n");			//串口初始化成功上报
	OLED_ShowString(48,6,"succeed! ");
	delay_ms(999);									//延时初界面显示
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
//	mpu6050.flag = 0;                               //采集成功标志位初始化
//	mpu6050.speed = 0;								//上报速度初始化
	delay_ms(2000);
	//初始化完成
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
//		MPU_Read();    //MPU6050数据读取
//		DATA_Report(); //MPU6050数据上报
		if(ADC_OK){
			ADC_VBAT = ADC_ConvertedValue[0]*3.3/4096;
			if(ADC_VBAT>=1.4&&ADC_VBAT<=2.1)
//				printf("此时电池的容量为%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
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
//					printf("此时电池的容量为%d%%\r\n",0);
					OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
			else
				if(ADC_VBAT>2.1)
//					printf("此时电池的容量为%d%%\r\n",100);		
					OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
		}
		if(MPU_OK)
		{
			MPU_OK =0;
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp处理得到数据，对返回值进行判断
			{ 
				temp=MPU_Get_Temperature();	                //得到温度值
				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
				if(temp<0)								//对数据正负判断，判断为负时
				{
					temp=-temp;							//对负数据取反
				}
				else                                    //判断为正时
				{
				}

				temp=pitch*10;							 //赋temp为pitch
				if(temp<0)								//对数据正负判断，判断为负时
				{
					temp=-temp;						    //对负数据取反		
				}
				else                                    //判断为正时 
				{
				}
//							printf(" pitch:%d.%d",temp/10,temp%10); //通过串口1输出pitch	
				OLED_ShowNum(56,2,temp/10,2,16);
				OLED_ShowChar(72,2,'.');
				OLED_ShowNum(80,2,temp%10,1,16);
				temp=roll*10;                            //赋temp为roll
				if(temp<0)								//对数据正负判断，判断为负时
				{
					temp=-temp;						    //对负数据取反	
				}
				else                                    //判断为正时
				{
				}
//							printf(" roll:%d.%d ",temp/10,temp%10);//通过串口1输出roll
				OLED_ShowNum(48,4,temp/10,2,16);
				OLED_ShowChar(64,4,'.');
				OLED_ShowNum(72,4,temp%10,1,16);
				temp=yaw*10;                           //赋temp为yaw
				if(temp<0)								//对数据正负判断，判断为负时
				{
					temp=-temp;						    //对负数据取反
				}
				else                                    //判断为正时
				{
				}
//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//通过串口1输出yaw	
				OLED_ShowNum(40,6,temp/10,2,16);
				OLED_ShowChar(56,6,'.');
				OLED_ShowNum(64,6,temp%10,1,16);
			}
		}
		Key_Value = KeyPressed();
		switch(Key_Value)
		{
			case 3://遥控和重力感应模式
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
//		printf("\r\n您发送的消息为:\r\n\r\n");
//		for(t=0;t<rx3_count;t++)
//		{
//			USART_SendData(USART1, BLE_RX[t]);//向串口1发送数据
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//		}
//		printf("\r\n\r\n");//插入换行'
//		printf("第四个是：%X，第五个是%X",BLE_RX[3],BLE_RX[4]);
		
//		if((BLE_RX[3] == 0xB2)&&(BLE_RX[4]==0xB6))//前进
//		{
//			car_Go(65,65);
//		}
//		else if((BLE_RX[3] == 0xB4)&&(BLE_RX[4]==0xB8))//向左转
//		{
//			car_Turn_Left(60,60);
//			delay_ms(200);//车无角度传感器，且由于车速不同，调整延时长短实现大概的转角 90度。
//			BLE_RX[3] = 0x00;//清缓存
//		}
//		else if((BLE_RX[3] == 0xB7)&&(BLE_RX[4]==0xBB))//左转圈
//		{
////					car_Left(60,60,60,60);
////					delay_ms(200);//车无角度传感器，且由于车速不同，调整延时长短实现大概的转角 90度。
////					BLE_RX[3] = 0x00;//清缓存
//		}
//		
//		else if((BLE_RX[3] == 0xB5)&&(BLE_RX[4]==0xB9))//停车
//		{
//			car_Stop();
//		}
//		else if((BLE_RX[3] == 0xB6)&&(BLE_RX[4]==0xBA))//向右转
//		{
//			car_Turn_Right(60,60);
//			delay_ms(200);//车无角度传感器，且由于车速不同，调整延时长短实现大概的转角 90度。
//			BLE_RX[3] = 0x00;//清缓存
//		}
//		else if((BLE_RX[3] == 0xB9)&&(BLE_RX[4]==0xBD))//右转圈
//		{
////					car_Right(60,60,60,60);
////					delay_ms(200);//车无角度传感器，且由于车速不同，调整延时长短实现大概的转角 90度。
////					BLE_RX[3] = 0x00;//清缓存
//		}
//		else if((BLE_RX[3] == 0xB8)&&(BLE_RX[4]==0xBC))//后退
//		{
//			car_Back(60,60);
//		} 
				
//		delay_ms(500);
						OLED_DrawBMP(0,0,16,1,Signal);		//信号图标
						if(ADC_OK){
							ADC_OK=0;
							ADC_VBAT = ADC_ConvertedValue[0]*3.3/4096;
							if(ADC_VBAT>=1.4&&ADC_VBAT<=2.1)
				//				printf("此时电池的容量为%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
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
				//					printf("此时电池的容量为%d%%\r\n",0);
									OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
							else
								if(ADC_VBAT>2.1)
				//					printf("此时电池的容量为%d%%\r\n",100);		
									OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
						}
						if(MPU_OK)
						{
							MPU_OK =0;
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp处理得到数据，对返回值进行判断
							{ 
								temp=MPU_Get_Temperature();	                //得到温度值
								MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
								MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;							//对负数据取反
								}
								else                                    //判断为正时
								{
								}

								temp=pitch*10;							 //赋temp为pitch
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反		
								}
								else                                    //判断为正时 
								{
								}
	//							printf(" pitch:%d.%d",temp/10,temp%10); //通过串口1输出pitch	
								OLED_ShowNum(56,2,temp/10,2,16);
								OLED_ShowChar(72,2,'.');
								OLED_ShowNum(80,2,temp%10,1,16);
								temp=roll*10;                            //赋temp为roll
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反	
								}
								else                                    //判断为正时
								{
								}
	//							printf(" roll:%d.%d ",temp/10,temp%10);//通过串口1输出roll
								OLED_ShowNum(48,4,temp/10,2,16);
								OLED_ShowChar(64,4,'.');
								OLED_ShowNum(72,4,temp%10,1,16);
								temp=yaw*10;                           //赋temp为yaw
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反
								}
								else                                    //判断为正时
								{
								}
	//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//通过串口1输出yaw	
								OLED_ShowNum(40,6,temp/10,2,16);
								OLED_ShowChar(56,6,'.');
								OLED_ShowNum(64,6,temp%10,1,16);
							}
						}
							if(USART3_RX_STA&0x8000)
							{					   
								len=USART3_RX_Count;//得到此次接收到的数据长度
								printf("\r\n您发送的消息为:\r\n\r\n");
								for(t=0;t<len;t++)
								{
									USART_SendData(USART1, USART3_RX_BUF[t]);//向串口1发送数据
									while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
								}
								printf("\r\n\r\n");//插入换行'
							if(strstr((char*)USART3_RX_BUF,"CM")!=NULL)		//重力感应模式
							{
								weight_flag =1;
								remote_flag =0;
							}
							else
							{
								weight_flag =0;		
							}
							if(strstr((char*)USART3_RX_BUF,"CJ")!=NULL)		//摇杆控制模式
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
								if(remote_flag)			//摇杆模式下
								{
									GetLocation((char*)USART3_RX_BUF,&x,&y);
									printf("x=%.3f,y=%.3f\r\n",x,y);//插入换行
									if((float)x==0&&(float)y != 0)	{
										if((float)y>0){
											Temp_y = GFP_abs((int)((float)y*1000));
											car_Go(Temp_y,Temp_y);	//全速前进
										}
										else if((float)y<0)
										{
											Temp_y = GFP_abs((int)((float)y*1000));
											car_Back(Temp_y,Temp_y);	//全速后退
										}
									}		
								 if (((float)x > 0 && (float)y < 0) || ((float)x > 0 && (float)y > 0)) { //第一、四象限方向  右转、右退  左轮>右轮
									if((float)y > 0)	//第一象限
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
									else if((float)y < 0)	//第四象限
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
								} else if (((float)x < 0 && (float)y  < 0) || ((float)x < 0 && (float)y > 0)) {//第二、三象限方向 左转、左退  右轮>左轮
									if((float)y > 0)	//第二象限
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
									else if((float)y < 0)	//第三象限
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
									car_Stop();		//小车停止
								}
							}
							if(weight_flag)		//重力感应模式	x:前后	y:左右
							{
								GetLocation((char*)USART3_RX_BUF,&x,&y);
								printf("x=%.3f,y=%.3f\r\n",x,y);//插入换行
								if((float)y==0&&(float)x!= 0)	{
									if((float)x>0){
										Temp_x = GFP_abs((int)((float)x*10));
										car_Go(Temp_x,Temp_x);	//全速前进
									}
									else if((float)x<0)
									{
										Temp_x = GFP_abs((int)((float)x*10));
										car_Back(Temp_x,Temp_x);	//全速后退
									}
								}		
								 if (((float)y > 0 && (float)x < 0) || ((float)y > 0 && (float)x > 0)) { //第一、四象限方向  右转、右退  左轮>右轮
									if((float)x > 0)	//第一象限
									{
										if((float)y>=(float)x){	//上正半轴
										Temp_x = GFP_abs((int)((float)x*10));	//前后倾角
										Temp_y = GFP_abs((int)((float)y*10));	//左右倾角
										car_Right(Temp_y,Temp_x);
									}
									else if((float)y<(float)x){	//下正半轴
										
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
									else if((float)x < 0)	//第四象限
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
								} else if (((float)y < 0 && (float)x  < 0) || ((float)y < 0 && (float)x > 0)) {//第二、三象限方向 左转、左退  右轮>左轮
									if((float)x > 0)	//第二象限
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
									else if((float)x < 0)	//第三象限
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
									car_Stop();		//小车停止
								}
							}									
								USART3_RX_STA=0;
								memset((char*)USART3_RX_BUF, 0, sizeof(USART3_RX_BUF)); 
							}
					}
					break;
			case 4:	//PID巡线模式
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
				//				printf("此时电池的容量为%d%%\r\n",(int)((float)((ADC_VBAT-1.4)/0.007)));
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
				//					printf("此时电池的容量为%d%%\r\n",0);
									OLED_DrawBMP(110,0,126,1,BatteryNone_BMP);
							else
								if(ADC_VBAT>2.1)
				//					printf("此时电池的容量为%d%%\r\n",100);		
									OLED_DrawBMP(110,0,126,1,BatteryFull_BMP);
						}
						if(MPU_OK)
						{
							MPU_OK =0;
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp处理得到数据，对返回值进行判断
							{ 
								temp=MPU_Get_Temperature();	                //得到温度值
								MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
								MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;							//对负数据取反
								}
								else                                    //判断为正时
								{
								}

								temp=pitch*10;							 //赋temp为pitch
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反		
								}
								else                                    //判断为正时 
								{
								}
	//							printf(" pitch:%d.%d",temp/10,temp%10); //通过串口1输出pitch	
								OLED_ShowNum(56,2,temp/10,2,16);
								OLED_ShowChar(72,2,'.');
								OLED_ShowNum(80,2,temp%10,1,16);
								temp=roll*10;                            //赋temp为roll
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反	
								}
								else                                    //判断为正时
								{
								}
	//							printf(" roll:%d.%d ",temp/10,temp%10);//通过串口1输出roll
								OLED_ShowNum(48,4,temp/10,2,16);
								OLED_ShowChar(64,4,'.');
								OLED_ShowNum(72,4,temp%10,1,16);
								temp=yaw*10;                           //赋temp为yaw
								if(temp<0)								//对数据正负判断，判断为负时
								{
									temp=-temp;						    //对负数据取反
								}
								else                                    //判断为正时
								{
								}
	//							printf(" yaw:%d.%d \r\n",temp/10,temp%10);//通过串口1输出yaw	
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
  * @brief  MPU6050数据读取函数
  * @param  无
  * @retval 无
  */
void MPU_Read(void)
{
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp处理得到数据，对返回值进行判断
	{ 
		temp=MPU_Get_Temperature();	                //得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		mpu6050.speed++;                            //上报速度自加
		if(mpu6050.speed == 1)						//上报速度阈值设置
		{
			mpu6050.flag = 1;						//采集成功标志位设置为有效
			mpu6050.speed = 0;						//上报速度归零
		}	
	}
	else 											//采集不成功										
	{
		mpu6050.flag = 0;							//采集成功标志位设置为无效
	}	
}
/**
  * @brief  MPU6050数据上报
  * @param  无
  * @retval 无
  */
void DATA_Report(void)
{
	if(mpu6050.flag == 1)									//采集成功时
	{
		mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//发送加速度传感器数据和陀螺仪数据
		usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));//上报结算后的姿态数据
		mpu6050.flag = 0;									//采集成功标志位设置为无效
	}
	else ;														//防卡死
}
