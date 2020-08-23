#include "key.h"
#include "lcd.h"
#include "i2c.h"
#include "delay.h"
#include "TIMER0.h"
/**************************************************************************************
*		              ���󰴼�ʵ��												  *
ʵ���������س�����������ʾ0�����¾��󰴼��ϵİ�����ʾ��Ӧ������
			S1-S4��0-3
			S5-S8��4-7
			S9-S12��8-B
			S13-S16��C-F��
ע�����																				  
***************************************************************************************/

#define GPIO_DIG P0
#define GPIO_KEY P1

sbit beep=P2^0;
sbit k1=P1^5;	 //����P15����k1
sbit k2=P1^6;
sbit k3=P1^7;


unsigned int password_1[6]={0,0,0,0,0,0};
unsigned int password_2[6]={0,0,0,0,0,0};
unsigned int password_3[6]={0,0,0,0,0,0}; //��ԭʼ����
unsigned int password_4[6]={0,0,0,0,0,0}; //��ڶ�������
unsigned char re_admin_password[4]={'0','0','0','0'}; //����Ա����


unsigned char KeyValue=20;	//������Ŷ�ȡ���ļ�ֵ
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
	if(k1==0)		  //��������
	{	
		delay_ms(10);
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			flag_2=1;
//			LcdWriteCom(0x01);
			password_input();

		}
		while(!k1);	 //��ⰴ���Ƿ��ɿ�
	}		
	if(k2==0)		  //��������
	{	
		delay_ms(10);
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			flag_1=1;
			flag=1;
			flag_3=1;
//			LcdWriteCom(0x01);
			password_original();
		}
		while(!k2);	 //��ⰴ���Ƿ��ɿ�
	}		
	if(k3==0)		  //�������Ա����
	{	
		delay_ms(10);  //�������� һ���Լ10ms
		if(k3==0)	 //�ٴ��жϰ����Ƿ���
		{
//			LcdWriteCom(0x01);
			KeyValue=20;
			flag_2=1;
			admin_password_input();
		}
		while(!k3);	 //��ⰴ���Ƿ��ɿ�
	}		
	EA=1;
}

/*******************************************************************************
* �� �� ��         : KeyDown
* ��������		   : ����а������²���ȡ��ֵ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//��ȡ�����Ƿ���
	{
		delay_ms(10);//��ʱ10ms��������
		if(GPIO_KEY!=0x0f)//�ٴμ������Ƿ���
		{	
			//������
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//������
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
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
			At24c02Write(1,password_4[0]);   //�ڵ�ַ1��д������num
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