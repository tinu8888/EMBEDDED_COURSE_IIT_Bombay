#include<P18f4550.h>
#include "boot.h"
void delay1(unsigned char count);
void main()
{
	TRISB=0X00;
	PORTB=0X00;
	while(1)
	{
			PORTB=0X00;
			delay1(3);
			PORTB=0XFF;
			delay1(2);
	}
}
void delay1(unsigned char count)
{	int i;
	for(i=0;i<=count;i++)
	{
	T0CON=0X08;
	TMR0H=0X00;
	TMR0L=0XC8;
	T0CONbits.TMR0ON=1;
	while(INTCONbits.TMR0IF==0);
	T0CONbits.TMR0ON=0;
	INTCONbits.TMR0IF=0;
}
}