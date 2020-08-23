#include "key.h"
#include "lcd.h"
#include "i2c.h"
#include "delay.h"
#include "TIMER0.h"
/**************************************************************************************
*		              矩阵按键实验												  *
实现现象：下载程序后数码管显示0，按下矩阵按键上的按键显示对应的数字
			S1-S4：0-3
			S5-S8：4-7
			S9-S12：8-B
			S13-S16：C-F。
注意事项：																				  
***************************************************************************************/

#define GPIO_DIG P0
#define GPIO_KEY P1

sbit beep=P2^0;
sbit k1=P1^5;	 //定义P15口是k1
sbit k2=P1^6;
sbit k3=P1^7;


unsigned int password_1[6]={0,0,0,0,0,0};
unsigned int password_2[6]={0,0,0,0,0,0};
unsigned int password_3[6]={0,0,0,0,0,0}; //存原始密码
unsigned int password_4[6]={0,0,0,0,0,0}; //存第二次密码
unsigned char re_admin_password[4]={'0','0','0','0'}; //管理员密码


unsigned char KeyValue=20;	//用来存放读取到的键值
unsigned char flag=1;
unsigned char flag_1=1;
unsigned char flag_2=1;
unsigned char flag_3=1;
unsigned char i;

unsigned char words1[11]="pass input:";
unsigned char words2[9]="pass set:";
unsigned char words3[10]="orig pass:";
unsigned char words4[15]="orig pass error";
unsigned char words5[7]="success";

void password_input(void);	
void match_password(void);					
void password_set(void);					
void password_original(void);
void admin_password_input(void);

void duju()
{
		password_4[0]=At24c02Read(1); 
		delay_ms(100);
		password_4[1]=At24c02Read(3);
		delay_ms(100);
		password_4[2]=At24c02Read(5);
		delay_ms(100);
		password_4[3]=At24c02Read(7);
		delay_ms(100);
		password_4[4]=At24c02Read(9);
		delay_ms(100);
		password_4[5]=At24c02Read(11);
		delay_ms(100);
}

void keypros()
{
	EA=0;
//	realy=0;
	if(k1==0)		  //输入密码
	{	
		delay_ms(10);
		if(k1==0)	 //再次判断按键是否按下
		{
			flag_2=1;
//			LcdWriteCom(0x01);
			password_input();

		}
		while(!k1);	 //检测按键是否松开
	}		
	if(k2==0)		  //设置密码
	{	
		delay_ms(10);
		if(k2==0)	 //再次判断按键是否按下
		{
			flag_1=1;
			flag=1;
			flag_3=1;
//			LcdWriteCom(0x01);
			password_original();
		}
		while(!k2);	 //检测按键是否松开
	}		
	if(k3==0)		  //输入管理员密码
	{	
		delay_ms(10);  //消除抖动 一般大约10ms
		if(k3==0)	 //再次判断按键是否按下
		{
//			LcdWriteCom(0x01);
			KeyValue=20;
			flag_2=1;
			admin_password_input();
		}
		while(!k3);	 //检测按键是否松开
	}		
	EA=1;
}

/*******************************************************************************
* 函 数 名         : KeyDown
* 函数功能		   : 检测有按键按下并读取键值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		delay_ms(10);//延时10ms进行消抖
		if(GPIO_KEY!=0x0f)//再次检测键盘是否按下
		{	
			//测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				delay_ms(10);
				a++;
			}
		}
	}

}

void admin_password_input()
{
	u8 b=0;
	LcdWriteCom(0x80+1);
	for(i=0;i<11;i++)
	{
		LcdWriteData(words1[i]);
	}
	while(flag_2)
	{
		KeyDown();
		if(KeyValue<10)
		{
			re_admin_password[b]=KeyValue+'0';
			LcdWriteCom(0x80+0x40+b);
			LcdWriteData('*');
			KeyValue=11;
			b++;
			if(b>=4)
				b=0;
		}
		if(KeyValue==10) 
		{
			flag_2=0;
			KeyValue=20;
			LcdWriteCom(0x01);
			
		}
	}
	LcdWriteCom(0x01);
	if(re_admin_password[0]=='1'&&re_admin_password[1]=='2'&&re_admin_password[2]=='3'&&re_admin_password[3]=='4')
	{
		LcdWriteCom(0x80+1);
		for(i=0;i<6;i++)
		{
			LcdWriteData(password_4[i]+'0');
		}

		flag_2=1;
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		LcdWriteCom(0x01);
	}
	else
	{
		LcdWriteCom(0x80);
		for(i=0;i<15;i++)
		{
			LcdWriteData(words4[i]);
		}
		flag_2=1;
		delay_ms(1000);
		LcdWriteCom(0x01);
	}
}


void match_password()
{
	u8 c;
	
	if(password_1[0]==password_4[0]&&password_1[1]==password_4[1]&&password_1[2]==password_4[2]&&password_1[3]==password_4[3]&&password_1[4]==password_4[4]&&password_1[5]==password_4[5])
	{
		realy=1;
		LcdWriteCom(0x80+0x40+5);		
		LcdWriteData('Y');
		delay_ms(1000);
		delay_ms(1000);
		LcdWriteCom(0x01);
		
		flag_2=0;
	}
	else
	{
		LcdWriteCom(0x80+0x40+5);
		LcdWriteData('N');
		flag_2=1;
		KeyValue=20;
		delay_ms(1000);
		LcdWriteCom(0x01);
		flag++;
		while(flag<=3)
			password_input();
		if(flag==4)
		{	
			realy=0;
			for(c=0;c<60;c++)
			{
				beep=0;
				delay_ms(500);
				beep=1;
				delay_ms(500);
				
			}
			flag=5;
			flag_2=0;
			
		}
	}

	
	
}


void password_input()
{
	u8 b=0;
	LcdWriteCom(0x80+1);
	for(i=0;i<11;i++)
	{
		LcdWriteData(words1[i]);
	}
	while(flag_2)
	{
		KeyDown();
		if(KeyValue<10)
		{
			password_1[b]=KeyValue;
			LcdWriteCom(0x80+0x40+b);
			LcdWriteData('*');
			KeyValue=11;
			b++;
			if(b>=6)
				b=0;
		}
		if(KeyValue==10) 
		{
			match_password();
			KeyValue=20;
			LcdWriteCom(0x01);
			
		}
	}
	
}


void password_set()
{
	u8 a=0;
	LcdWriteCom(0x80+1);
	for(i=0;i<9;i++)
	{
		LcdWriteData(words2[i]);
	}
	while(flag_1)
	{
		KeyDown();
		if(KeyValue<10)
		{
			password_2[a]=KeyValue;
			LcdWriteCom(0x80+0x40+a);
			LcdWriteData(KeyValue+'0');
			KeyValue=11;
			a++;
			if(a>=6)
				a=0;
		}
		if(KeyValue==10) 
		{
			
			KeyValue=20;
			LcdWriteCom(0x01);
			
			LcdWriteCom(0x80+1);
			for(i=0;i<9;i++)
			{
				LcdWriteData(words2[i]);
			}
			while(flag_1)
			{
				KeyDown();
				if(KeyValue<10)
				{
					password_4[a]=KeyValue;
					LcdWriteCom(0x80+0x40+a);
					LcdWriteData(KeyValue+'0');
					KeyValue=11;
					a++;
					if(a>=6)
						a=0;
				}
				if(KeyValue==10) 
				{
					flag_1=0;
					KeyValue=20;
					LcdWriteCom(0x01);
				}
				
			}
		
		if(password_4[0]==password_2[0]&&password_4[1]==password_2[1]&&password_4[2]==password_2[2]&&password_4[3]==password_2[3]&&password_4[4]==password_2[4]&&password_4[5]==password_2[5])
		{
			flag_3=1;
			LcdWriteCom(0x80);
			for(i=0;i<7;i++)
			{
				LcdWriteData(words5[i]);
			}
			At24c02Write(1,password_4[0]);   //在地址1内写入数据num
			delay_ms(100);
			At24c02Write(3,password_4[1]);
			delay_ms(100);
			At24c02Write(5,password_4[2]);
			delay_ms(100);
			At24c02Write(7,password_4[3]);
			delay_ms(100);
			At24c02Write(9,password_4[4]);
			delay_ms(100);
			At24c02Write(11,password_4[5]);
			
			delay_ms(1000);
			delay_ms(1000);
			LcdWriteCom(0x01);
		}
		else
		{
			flag_3=1;
			LcdWriteCom(0x80);
			for(i=0;i<15;i++)
			{
				LcdWriteData(words4[i]);
			}
			duju();
			delay_ms(1000);
			delay_ms(1000);
			LcdWriteCom(0x01);
		}
	}
	}

}


void password_original()
{
	u8 a=0;
	LcdWriteCom(0x80+1);
	for(i=0;i<10;i++)
	{
		LcdWriteData(words3[i]);
	}
	while(flag_3)
	{
		KeyDown();
		if(KeyValue<10)
		{
			password_3[a]=KeyValue;
			LcdWriteCom(0x80+0x40+a);
			LcdWriteData(KeyValue+'0');
			KeyValue=11;
			a++;
			if(a>=6)
				a=0;
		}
		if(KeyValue==10) 
		{
			flag_3=0;
			KeyValue=20;
			LcdWriteCom(0x01);
		}
		
	}
	if(password_3[0]==password_4[0]&&password_3[1]==password_4[1]&&password_3[2]==password_4[2]&&password_3[3]==password_4[3]&&password_3[4]==password_4[4]&&password_3[5]==password_4[5])
	{
		flag_3=1;
		LcdWriteCom(0x01);
		password_set();
	}
	else
	{
		flag_3=1;
		LcdWriteCom(0x80);
		for(i=0;i<15;i++)
		{
			LcdWriteData(words4[i]);
		}
		delay_ms(1000);
		delay_ms(1000);
		LcdWriteCom(0x01);
	}
}