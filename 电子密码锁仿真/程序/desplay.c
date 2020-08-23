#include "display.h"
#include <reg51.h>
#include "main.h"

sbit w1 = P2^2;
sbit w2 = P2^3;

uchar code Tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};


void display(uchar value)
{
	w1 = 0;
	P1 = Tab[value/10];   
	delay(1);
	w1 = 1;
	P1 = 0;
	delay(1);
	w2 = 0;
	P1 = Tab[value%10];
	delay(1);
	w2 = 1;
	P1 = 0;
	delay(1);
	
}
