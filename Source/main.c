/**********************************************************************************
 
* ������  :����������
* ����    :ͨ����GSM�巢�Ϳ���������ţ������Ƶ�Ƭ�����ϵ�LED�ƣ���������Ϊ��on ����off�� 
* ʵ��ƽ̨:STC89XX
*Ӳ�������ӣ�
4*4���󰴼�    ��ɫ��P1^0����˳������
�̵���   + 5V
				 - GND
				 S P2^4
������  VCC 5V
				I/O P2^0
				GND GND
AT24C02   SCL P2^3
					SDA P2^2
					VCC 5V
					GND GND
SIM900A   ��������USB�� 5V 2A
					5VR RX
					5VT TX
					VCC_MCU 5V(�ɲ���)
					GND GND
��Ƭ�� 5V ����

**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"
#include "timer0.h"
#include "SIMxxx.h"
#include "key.h"
#include "lcd.h"

sbit RUNING_LED=P2^1;	//�����п��Ƶ�LED��

/*************  �ⲿ�����ͱ�������*****************/
/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void main(void)
{
	u8 res;
	Uart1Init();
	Timer0Init();
	LcdInit();
	EA=1;	//�����ж�
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




