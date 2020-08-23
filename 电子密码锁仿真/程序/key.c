#include <reg51.h>
#include "main.h"
#include "display.h"


/******************************************
 计算器按键扫描
******************************************/
uchar temp = 0xff;


uchar Calkey_scan()
{
	//**********扫描第一行*********
	uchar tmp = 0xff;
	P1=0xfe;
	temp=P1;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(50);
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
				case 0xee:
					tmp = 1;
				break;
				
				case 0xde:
					tmp = 2;
				break;
				
				case 0xbe:
					tmp = 3;
				break;
				
				case 0x7e:
					tmp = 4;
				break;
			}
			while(temp!=0xf0)
			{
				temp = P1;
				temp = temp&0xf0;
			}
	
		}
	}


	//**********扫描第二行*********
	P1=0xfd;
	temp=P1;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(50);
		   temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
			case 0xed:
				tmp = 5;
			break;
			
			case 0xdd:
				tmp = 6;
			break;
			
			case 0xbd:
				tmp = 7;
			break;
			
			case 0x7d:
				tmp = 8;
			break;
			}
			while(temp!=0xf0)
			{
				temp = P1;
				temp = temp&0xf0;
			}
		
		}
	}


	//**********扫描第三行*********
	P1=0xfb;
	temp=P1;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(50);
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
				case 0xeb:
				tmp = 9;
				break;
				
				case 0xdb:
				tmp = 0;
				break;
				
				case 0xbb:
				tmp = 11;
				break;
				
				case 0x7b:
				tmp = 12;
				break;
			}
			while(temp!=0xf0)
			{
				temp = P1;
				temp = temp&0xf0;
			}
		}
		
	}

	//**********扫描第四行*********
	P1=0xf7;
	temp=P1;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(50);
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
			case 0xe7:
				tmp = 13;
			break;
			
			case 0xd7:
				tmp =14;
			break;
			
			case 0xb7:
				tmp = 15;
			break;
			
			case 0x77:
				tmp = 16;
			break;
			}
			
			while(temp!=0xf0)
			{
				temp = P1;
				temp = temp&0xf0;
			}
			
		}
	}
	return tmp;

}