#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"		
#include "sys.h"
#include "oled.h"
#include "pid.h"
#include "hardware.h"

//extern void car_Go(u16 moto_l, u16 moto_r);
//extern float PID_OutPut();
//extern void led_set(u8 sta);
//extern void test_fun(void(*ledset)(u8),u8 sta);
 
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)", 	
	(void*)OLED_Clear,"void OLED_Clear(void)",
//	(void*)OLED_Fill,"void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)",
	(void*)OLED_ShowChar,"void OLED_ShowChar(u8 x,u8 y,u8 chr)",
	(void*)OLED_ShowNum,"void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)",
	(void*)OLED_ShowString,"void OLED_ShowString(u8 x,u8 y, u8 *p)",
	(void*)OLED_Set_Pos,"void OLED_Set_Pos(unsigned char x, unsigned char y)",
	(void*)OLED_ShowCHinese,"void OLED_ShowCHinese(u8 x,u8 y,u8 no)",	
//	(float*)PID_OutPut,"float PID_OutPut(float Kp,float Ki,float Kd)",
//	(void*)car_Go,"void car_Go(u16 moto_l, u16 moto_r)",				  	    
	(void*)OLED_DrawBMP,"void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















