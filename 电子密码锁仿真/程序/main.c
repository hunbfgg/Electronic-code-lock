#include <reg51.h>
#include <intrins.h>
#include <stdlib.h>
#include <string.h>
#include<absacc.h>
#include "lcd1602.h"
#include "main.h"
#include "key.h"

sbit Lock = P3^7;  //代表锁控的继电器IO   高电平关锁，低电平开锁
sbit LEDRED = P2^0;  //代表密码输入错误的红灯IO  高电平熄灭，低电平点亮
sbit LEDGREEN = P2^1;  //代表密码输入正确的绿灯IO  高电平熄灭，低电平点亮
sbit beep=P1^5;	 
uchar genPasswd[4];           //存储生成的四位随机数密码
uchar inpPasswd[4];           //存储取包时输入的四位密码
uchar code welcome[8]="welcome";  //开机显示的欢迎标语
uchar code set[7]="Passwd";    //显示生成的密码标题
uchar code  close[12]="close door!";   //提示关门
uchar code 	input[13] = "input passwd";   //提示请输入密码

/****************定时器设置相关的变量***********************/
unsigned char TL0_temp;	 /*暂存TL0的初值*/
unsigned char TH0_temp;	 /*暂存TH0的初值*/
#define INT_CLOCK 10    /*INT_CLOCK为定时值，单位为ms ,此处定义为10ms*/
#define CRY_FREQUENCY 11059200 /*CRY_FREQUENCY为晶振频率，单位为Hz*/





/*延时程序*/
void delay(uint z)		
{
	unsigned int i,j;
   for(i=0;i<z;i++)
    for(j=0;j<100;j++);
}
/******************************
显示首页 welcome
******************************/
void display1()
{
   unsigned char i;
   for(i = 0;i < 7;i++)
   {
   		wr_com(0x85+i);
		wr_dat(welcome[i]);
   }
}


/************************************
显示密码生成界面
************************************/
void display2()
{
   unsigned char i;
   
   for(i = 0;i < 6;i++)
   {
   		wr_com(0x85+i);
		wr_dat(set[i]);
   }
   for(i = 0;i < 4;i++)
   {
   		wr_com(0xc6+i);
		wr_dat(genPasswd[i]+0x30);
		
   }
}

/************************************
显示请关门界面
************************************/

void display3()
{
     unsigned char i;
   for(i = 0;i < 11;i++)
   {
   		wr_com(0x83+i);
		wr_dat(close[i]);
   }

}


/************************************
显示密码输入界面
************************************/
void display4()
{
   unsigned char i;
   for(i = 0;i < 12;i++)
   {
   		wr_com(0x82+i);
		wr_dat(input[i]);
   }
   for(i = 0;i < 4;i++)
   {
   		wr_com(0xc6+i);
		wr_dat(inpPasswd[i]+0x30);
		
   }

}
	




/***********************************
定时器0初始化
***********************************/
void Timer0_init()
{
	unsigned long T0_temp;                                     /*暂存T0的初始值	*/
	T0_temp = 65536-((INT_CLOCK *CRY_FREQUENCY /1000)/12);    /*T0初始值计算公式，参考教科书*/
	TL0_temp = T0_temp & 0xff;
	TH0_temp = T0_temp >> 8;
	TL0 = TL0_temp;
	TH0 = TH0_temp;
	TMOD |= 0x01;
	TR0 = 1;  	              /*开定时器0中断*/
	ET0 = 1;  
	EA = 1;                    /*开总中断*/                                       

}






/*主函数*/
uchar global_state = 0;
uchar count_num = 0;  //输入的数字个数
void main()
{  
   uchar i,j;
   uchar passwd;
    
   	uchar key_value = 0;
	uchar temp_key_value = 0xff;
	Lock = 1;  //刚上电 锁是需要保持闭合
	Timer0_init();	//定时器初始化
 	lcd_init();	//LCD1602初始化
   
	while(1)	//进入不断循环
	{
	   	temp_key_value = Calkey_scan();	  //读取矩阵按键的键值
		if(temp_key_value != 0xff)
		{
			key_value = temp_key_value;
		}
		switch(global_state)
		{
			case 0:	 //初始化状态
				display1();	 //显示欢迎语句	-- "welcome"
			

				if(key_value == 13)  //存包按键被按下
				{
				    key_value = 0xff;  //将按键值清除
					Lock = 0;    //开锁
					
					display3();    //显示关门提示语	-- "close door!"
				    delay(1900);    //等待关门 大约3秒
					Lock = 1;   //关门
					clear();		 //清屏，准备下一屏的显示
					global_state = 1;   //切换运行状态
					passwd = TL0;  //采用定时器寄存器的值作为密码
					genPasswd[0] = passwd/100;		  //存储密码
					genPasswd[1] = passwd%100/10;
					genPasswd[2] = passwd%10;
					genPasswd[3] = (genPasswd[1]+genPasswd[2])%10;
				} 
				if(key_value == 14)  //取包按键被按下
				{
				    key_value = 0xff;  //将按键值清除
					clear();    //清屏，准备下一屏的显示
					global_state = 2;  //切换运行状态
					count_num = 0;   //清除数字按键变量
					for(i = 0;i < 4;i++)  //清空输入的密码数组
					    inpPasswd[i] = ' '-0x30;
				} 
			break;
			case 1:		//密码生成状态
			   	display2();	 //显示密码生成界面
				delay(1900);    //等待大约3秒
				global_state = 0;  //返回到初始界面
				clear();	//清屏，准备下一屏的显示
				
			break;
			case 2:	  //输入密码状态
			   display4();	   //显示输入密码界面
			   if((key_value >= 0) && (key_value <= 9))	  //如果按下的为0-9的数字按键
			   {
			       
			        if(count_num <= 3)	 //如果按键次数小于4次
					{
				        inpPasswd[count_num] = key_value; //将按键值存入数组,并显示
				   		count_num++;
				    }
					 key_value = 0xff;  //将按键值清除
			   }
			   if(key_value == 15) //清除按键被按下
			   {
			        key_value = 0xff;  //将按键值清除
			   		for(i = 0;i < 4;i++)
					  inpPasswd[i] = ' '-0x30;  //清除刚刚输入的密码
			   }
			   if(key_value == 16) //确认按键被按下
			   {
			        key_value = 0xff;  //将按键值清除
			   		for(i = 0;i < 4;i++)
					{
						if(inpPasswd[i] != genPasswd[i])
						   break;
					}
					if(i == 4)	  //密码输入正确
					{
						LEDRED = 1;	 //红灯熄灭
						LEDGREEN = 0;  //绿灯亮
						Lock = 1;  //开锁
						delay(1900);    //等待大约3秒
						Lock = 0;  //关锁
						LEDGREEN = 1;  //绿灯熄灭
					}
					else   //密码输入错误
					{	

						for(j = 0;j <19009 ;j++)
						{
							beep=~beep;
							delay(10);
							LEDRED = 0;	 //红灯亮
							LEDGREEN = 1;  //绿灯熄灭
							 continue;
						 } 
						Lock = 0;  //关锁
						delay(1900);    //等待大约3秒
						LEDRED = 1;  //红灯熄灭
					}
					clear();   	//清屏，准备下一屏的显示
					global_state = 0;  //返回到初始界面

			   }
			break;	
		}
	}
}


void T0_interrupt(void) interrupt 1	 
{
	TH0 += TH0_temp;
	TL0 += TL0_temp;

}



	




