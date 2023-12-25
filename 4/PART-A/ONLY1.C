
#include<P18f4550.h>
#include "boot.h" 	
void delay(unsigned char count);
#define pwm PORTBbits.RB2
#define sw1 PORTBbits.RB5
void main()
{	pwm=0;
	TRISB=0xf0;	
	while(1)
	{
		if(sw1==1)
		{
			pwm=1;
			delay(1);
			pwm=0;
			delay(4);			
		}
	/*	if(sw2==1)
		{
			pwm=1;
			delay(2);
			pwm=0;
			delay(3);	}
		if(sw3==1)
		{
			pwm=1;
			delay(3);
			pwm=0;
			delay(2);	}
	*/
	else
	{
	pwm=0;
	
	}
	}
	
}
void delay(unsigned char count)
{	int i;
	for(i=0;i<count;i++)
	{
	T0CON=0X08;
	TMR0H=0XFF;
	TMR0L=0X38;
	T0CONbits.TMR0ON=1;
	while(INTCONbits.TMR0IF==0);
	T0CONbits.TMR0ON=0;
	INTCONbits.TMR0IF=0;
}
}