#include<P18f4550.h>
#include "boot.h"
#include "lcd.h"
int  i;
void T0_isr1(void);
void pwm20(void);
void pwm40(void);
void pwm60(void);
void adc();
unsigned long voltagel,voltageh,vlt;
char digit[4];

#pragma interrupt chk_isr
void chk_isr(void)
{
	if (INTCONbits.TMR0IF==1)
	T0_isr1();
}
#pragma code
void main()
{
	
	char str1[] = "voltage = ";
		i=0;
	INTCON2bits.RBPU=0;
	TRISB=0x00;	
	TRISAbits.TRISA0=1;
	lcdint();
	lcdstring(str1);
	T0CON=0X08;
	TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	INTCONbits.TMR0IE=1;
	INTCON2bits.TMR0IP=1;
	INTCONbits.PEIE=1;
	INTCONbits.GIE=1;
	T0CONbits.TMR0ON=1;
	while(1)
	{
			ADCON0=0x01;
			ADCON1=0x0e;
			ADCON2=0xCE;
			adc();
	}
}
void T0_isr1(void)
{	if(vlt<215)
		{
			pwm20();
		}
	else if(vlt<409&&vlt>215)
		{
			pwm40();
		}
	else if(vlt>409)
		{
			pwm60();
		}
	
	else
	{
		LATBbits.LATB2=0;
    	TMR0H=0XFF;
		TMR0L=0X38;
		INTCONbits.TMR0IF=0;
	}	
	
}	
void pwm20(void)
{		i++;
		if(i==5)
		{
			i=0;
		}
	if(i<1)
	{
	LATBbits.LATB2=1;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
	else
	{
	LATBbits.LATB2=0;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
}
void pwm40(void)
{		i++;
		if(i==5)
		{
			i=0;
		}
	if(i<2)
	{
	LATBbits.LATB2=1;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
	else
	{
	LATBbits.LATB2=0;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
}
void pwm60(void)
{		i++;
		if(i==3)
		{
			i=0;
		}
	if(i<2)
	{
	LATBbits.LATB2=1;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
	else
	{
	LATBbits.LATB2=0;
    TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	}
}

void adc()
{	lcdcmd(0xc0);
	ADCON0bits.GO=1;
	while(ADCON0bits.DONE==1);
	voltagel=ADRESL;
	voltageh=ADRESH;
	voltageh=voltageh<<8;
	vlt=voltagel|voltageh;
	digit[0]=(vlt/1000)+0x30;
	digit[1]=((vlt%1000)/100)+0x30;
	digit[2]=((vlt%100)/10)+0x30;
	digit[3]=(vlt%10)+0x30;
	lcddata(digit[0]);
	lcddata('.');
	lcddata(digit[1]);
	lcddata(digit[2]);
	lcddata(digit[3]);
	lcddata('v');	
}
