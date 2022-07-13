/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 


 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

void USART3_IRQHandler(void) 
{
	uint8_t ucTemp;
//	uint16_t Temp = oxodoa;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USART3);		
//		if(rx3_length > rx3_count)
//		{
//			*rx3_address = ucTemp;
//			rx3_address++;
//			rx3_count++;	
//		}
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		USART_SendData(USART3,ucTemp);
//		USART_SendData(USART1,ucTemp);
		
		if((USART3_RX_STA&0x8000)==0)	//接收未完成(接收到0x3B表示完成)
		{
			if(USART3_RX_STA&0x4000)//接收到了0x23，帧头数据
			{
				if(ucTemp==0x3B){	
					USART3_RX_STA|=0x8000;	//接收完成了
					
				}else
				{						
					USART3_RX_BUF[USART3_RX_Count]=ucTemp;
					USART3_RX_Count++;
					if(USART3_RX_Count>(USART3_MAX_RECV_LEN-1))
					USART3_RX_Count=0;//接收数据错误,重新开始接收
				}
	
			}
			else //没接收到0x23
			{
				if(ucTemp==0x23)	
				{
					USART3_RX_STA|=0x4000;
					USART3_RX_Count=0;
				}		
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		cnt++;
		cnt2++;
		cnt3++;
		if(cnt>=50)	//	1ms*1000 = 1s
		{
			cnt =0;
			MPU_OK =1;
		}
		if(cnt2>=10)	//1ms*10 = 10ms
		{
			cnt2 = 0;
			if(ADC_OK ==1)
		{
			ADC_OK=0;
//			printf("此时电池的电压为%fV\r\n",ADC_ConvertedValue[0]*3.3/4096);

//			printf("A5此时的AD值为%d\r\n",ADC_ConvertedValue[1]);
//			printf("A4此时的AD值为%d\r\n",ADC_ConvertedValue[2]);
//			printf("A3此时的AD值为%d\r\n",ADC_ConvertedValue[3]);
//			printf("A2此时的AD值为%d\r\n",ADC_ConvertedValue[4]);
//			printf("A1此时的AD值为%d\r\n",ADC_ConvertedValue[5]);	

		}
		}
		if(cnt3>=200)	//1ms*50 = 50ms
		{
			cnt3 = 0;
			trace_OK =1;

		}
	}
	
}


void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{
		ADC_OK = 1;
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
