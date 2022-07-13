#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"		
#include "sys.h"
#include "oled.h"
#include "pid.h"
#include "hardware.h"

//extern void car_Go(u16 moto_l, u16 moto_r);
//extern float PID_OutPut();
//extern void led_set(u8 sta);
//extern void test_fun(void(*ledset)(u8),u8 sta);
 
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
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
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















