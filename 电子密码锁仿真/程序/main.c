#include <reg51.h>
#include <intrins.h>
#include <stdlib.h>
#include <string.h>
#include<absacc.h>
#include "lcd1602.h"
#include "main.h"
#include "key.h"

sbit Lock = P3^7;  //�������صļ̵���IO   �ߵ�ƽ�������͵�ƽ����
sbit LEDRED = P2^0;  //���������������ĺ��IO  �ߵ�ƽϨ�𣬵͵�ƽ����
sbit LEDGREEN = P2^1;  //��������������ȷ���̵�IO  �ߵ�ƽϨ�𣬵͵�ƽ����
sbit beep=P1^5;	 
uchar genPasswd[4];           //�洢���ɵ���λ���������
uchar inpPasswd[4];           //�洢ȡ��ʱ�������λ����
uchar code welcome[8]="welcome";  //������ʾ�Ļ�ӭ����
uchar code set[7]="Passwd";    //��ʾ���ɵ��������
uchar code  close[12]="close door!";   //��ʾ����
uchar code 	input[13] = "input passwd";   //��ʾ����������

/****************��ʱ��������صı���***********************/
unsigned char TL0_temp;	 /*�ݴ�TL0�ĳ�ֵ*/
unsigned char TH0_temp;	 /*�ݴ�TH0�ĳ�ֵ*/
#define INT_CLOCK 10    /*INT_CLOCKΪ��ʱֵ����λΪms ,�˴�����Ϊ10ms*/
#define CRY_FREQUENCY 11059200 /*CRY_FREQUENCYΪ����Ƶ�ʣ���λΪHz*/





/*��ʱ����*/
void delay(uint z)		
{
	unsigned int i,j;
   for(i=0;i<z;i++)
    for(j=0;j<100;j++);
}
/******************************
��ʾ��ҳ welcome
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
��ʾ�������ɽ���
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
��ʾ����Ž���
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
��ʾ�����������
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
��ʱ��0��ʼ��
***********************************/
void Timer0_init()
{
	unsigned long T0_temp;                                     /*�ݴ�T0�ĳ�ʼֵ	*/
	T0_temp = 65536-((INT_CLOCK *CRY_FREQUENCY /1000)/12);    /*T0��ʼֵ���㹫ʽ���ο��̿���*/
	TL0_temp = T0_temp & 0xff;
	TH0_temp = T0_temp >> 8;
	TL0 = TL0_temp;
	TH0 = TH0_temp;
	TMOD |= 0x01;
	TR0 = 1;  	              /*����ʱ��0�ж�*/
	ET0 = 1;  
	EA = 1;                    /*�����ж�*/                                       

}






/*������*/
uchar global_state = 0;
uchar count_num = 0;  //��������ָ���
void main()
{  
   uchar i,j;
   uchar passwd;
    
   	uchar key_value = 0;
	uchar temp_key_value = 0xff;
	Lock = 1;  //���ϵ� ������Ҫ���ֱպ�
	Timer0_init();	//��ʱ����ʼ��
 	lcd_init();	//LCD1602��ʼ��
   
	while(1)	//���벻��ѭ��
	{
	   	temp_key_value = Calkey_scan();	  //��ȡ���󰴼��ļ�ֵ
		if(temp_key_value != 0xff)
		{
			key_value = temp_key_value;
		}
		switch(global_state)
		{
			case 0:	 //��ʼ��״̬
				display1();	 //��ʾ��ӭ���	-- "welcome"
			

				if(key_value == 13)  //�������������
				{
				    key_value = 0xff;  //������ֵ���
					Lock = 0;    //����
					
					display3();    //��ʾ������ʾ��	-- "close door!"
				    delay(1900);    //�ȴ����� ��Լ3��
					Lock = 1;   //����
					clear();		 //������׼����һ������ʾ
					global_state = 1;   //�л�����״̬
					passwd = TL0;  //���ö�ʱ���Ĵ�����ֵ��Ϊ����
					genPasswd[0] = passwd/100;		  //�洢����
					genPasswd[1] = passwd%100/10;
					genPasswd[2] = passwd%10;
					genPasswd[3] = (genPasswd[1]+genPasswd[2])%10;
				} 
				if(key_value == 14)  //ȡ������������
				{
				    key_value = 0xff;  //������ֵ���
					clear();    //������׼����һ������ʾ
					global_state = 2;  //�л�����״̬
					count_num = 0;   //������ְ�������
					for(i = 0;i < 4;i++)  //����������������
					    inpPasswd[i] = ' '-0x30;
				} 
			break;
			case 1:		//��������״̬
			   	display2();	 //��ʾ�������ɽ���
				delay(1900);    //�ȴ���Լ3��
				global_state = 0;  //���ص���ʼ����
				clear();	//������׼����һ������ʾ
				
			break;
			case 2:	  //��������״̬
			   display4();	   //��ʾ�����������
			   if((key_value >= 0) && (key_value <= 9))	  //������µ�Ϊ0-9�����ְ���
			   {
			       
			        if(count_num <= 3)	 //�����������С��4��
					{
				        inpPasswd[count_num] = key_value; //������ֵ��������,����ʾ
				   		count_num++;
				    }
					 key_value = 0xff;  //������ֵ���
			   }
			   if(key_value == 15) //�������������
			   {
			        key_value = 0xff;  //������ֵ���
			   		for(i = 0;i < 4;i++)
					  inpPasswd[i] = ' '-0x30;  //����ո����������
			   }
			   if(key_value == 16) //ȷ�ϰ���������
			   {
			        key_value = 0xff;  //������ֵ���
			   		for(i = 0;i < 4;i++)
					{
						if(inpPasswd[i] != genPasswd[i])
						   break;
					}
					if(i == 4)	  //����������ȷ
					{
						LEDRED = 1;	 //���Ϩ��
						LEDGREEN = 0;  //�̵���
						Lock = 1;  //����
						delay(1900);    //�ȴ���Լ3��
						Lock = 0;  //����
						LEDGREEN = 1;  //�̵�Ϩ��
					}
					else   //�����������
					{	

						for(j = 0;j <19009 ;j++)
						{
							beep=~beep;
							delay(10);
							LEDRED = 0;	 //�����
							LEDGREEN = 1;  //�̵�Ϩ��
							 continue;
						 } 
						Lock = 0;  //����
						delay(1900);    //�ȴ���Լ3��
						LEDRED = 1;  //���Ϩ��
					}
					clear();   	//������׼����һ������ʾ
					global_state = 0;  //���ص���ʼ����

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



	




