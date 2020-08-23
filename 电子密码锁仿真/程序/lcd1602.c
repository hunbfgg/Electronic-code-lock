#include "lcd1602.h"
#include <reg51.h>
#include "main.h"

/************************
 LCD相关变量
*************************/
sbit RS=P2^6;
sbit RW=P2^5;
sbit EN=P2^7;




/******************************************
 延时1ms
******************************************/
 void delay1ms(unsigned int ms)
{
	unsigned int i,j;
   for(i=0;i<ms;i++)
    for(j=0;j<100;j++);
}

/*发命令*/
void wr_com(unsigned char com)
{ 
	unsigned char temp;
	temp = com;
   delay1ms(1);
   RS=0;
   RW=0;
   EN=0;
   P0=temp;
   delay1ms(1);
   EN=1;
   delay1ms(1);
   EN=0;
}

/*写数据*/
void wr_dat(unsigned char dat)
{ 
   unsigned char temp;
	temp = dat;
   delay1ms(1);
   RS=1;
   RW=0;
   EN=0;
   P0=temp;
   delay1ms(1);
   EN=1;
   delay1ms(1);
   EN=0;
}

/*LCD1602初始化*/
void lcd_init()
{
	delay1ms(15);
	wr_com(0x38);delay1ms(5);
	wr_com(0x08);delay1ms(5);
	wr_com(0x01);delay1ms(5);
	wr_com(0x06);delay1ms(5);
	wr_com(0x0c);delay1ms(5);
}

void clear()//清屏
{ 
	uchar i;
	for(i = 0;i < 16;i++)
	{
		wr_com(0x80+i);
		wr_dat(' ');
		wr_com(0xc0+i);
		wr_dat(' ');
	}
}
