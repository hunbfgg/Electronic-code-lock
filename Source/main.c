/**********************************************************************************
 
* 工程名  :电子密码锁
* 描述    :通过向GSM板发送控制命令短信，来控制单片机板上的LED灯，控制命令为：on 或是off。 
* 实验平台:STC89XX
*硬件及连接：
4*4矩阵按键    红色接P1^0，按顺序往后
继电器   + 5V
				 - GND
				 S P2^4
蜂鸣器  VCC 5V
				I/O P2^0
				GND GND
AT24C02   SCL P2^3
					SDA P2^2
					VCC 5V
					GND GND
SIM900A   独立供电USB口 5V 2A
					5VR RX
					5VT TX
					VCC_MCU 5V(可不接)
					GND GND
单片机 5V 供电

**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"
#include "timer0.h"
#include "SIMxxx.h"
#include "key.h"
#include "lcd.h"

sbit RUNING_LED=P2^1;	//程序中控制的LED灯

/*************  外部函数和变量声明*****************/
/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void main(void)
{
	u8 res;
	Uart1Init();
	Timer0Init();
	LcdInit();
	EA=1;	//开总中断
	realy=0;
	res=1;
	RUNING_LED=0;
	while(res)
	{
		res=sim900a_work_test();
	}
	res=1;
	while(res)
	{
		res=sim900a_enmessage_mode_out();
	}
	RUNING_LED=1;
	res=1;
	duju();
	while(1)
	{
		
	if(Flag_Rec_Message==1)	
	{
		Flag_Rec_Message=0;
		res=SIM_HANDLE_MESSAGE_CMD();
		if(res)
		{
			if(res==1)		realy=1;
			else if(res==2)	realy=0;
		}
		
	}		
	keypros();
		
	}	
}




