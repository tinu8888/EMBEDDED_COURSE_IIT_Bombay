#include<P18f4550.h>
#include "boot.h"
#include "lcd.h"
#define pwm PORTBbits.RB2
void sine(unsigned long vrms);
unsigned long voltagel,voltageh,vlt,vltp;
char digit[4];
void delay1(unsigned char count);
void adc();
void main()
{	char str1[] = "voltage = ";
	TRISB=0x00;	
	TRISAbits.TRISA0=1;
	lcdint();
	lcdstring(str1);
	pwm=0;
	while(1)
	{
			ADCON0=0x01;
			ADCON1=0x0e;
			ADCON2=0xCE;
			adc();
			
	}
}
void adc()
{		lcdcmd(0xc0);
	LATBbits.LATB2=1;
			delay1(1);
			LATBbits.LATB2=0;
			delay1(4);
	ADCON0bits.GO=1;
	while(ADCON0bits.DONE==1);
	voltagel=ADRESL;
	voltageh=ADRESH;
	voltageh=voltageh<<8;
	vlt=voltagel|voltageh;
/*/	vlt=vlt*3000;
//	vlt=vlt/1023;
	if(vltp<=vlt)
	{ADCON0bits.GO=0;
	vltp=vlt;
	digit[0]=(vltp/1000)+0x30;
	digit[1]=((vltp%1000)/100)+0x30;
	digit[2]=((vltp%100)/10)+0x30;
	digit[3]=(vltp%10)+0x30;
	lcddata(digit[0]);
//	lcddata('.');
	lcddata(digit[1]);
	lcddata(digit[2]);
	lcddata(digit[3]);
//	lcddata('v');
//	sine(100);	
*/	

	
}
void delay1(unsigned char count)
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
void sine(unsigned long vrms)
{
if(vrms<409)
		{
			
		}
		if((vrms<818)&&(vrms>409))
		{
				pwm=1;
			delay1(2);
			pwm=0;
			delay1(3);
	
		}
		if((vrms<1023)&&(vrms>818))
		{
				pwm=1;
			delay1(3);
			pwm=0;
			delay1(2);
		}

}