#include<P18f4550.h>
#include "boot.h"
#include "lcd.h"
void adc();
unsigned long voltagel,voltageh,vlt;
char digit[4];
void main()
{	char str1[] = "DIGITALVOLTMETER";
	char str2[] = "voltage = ";
	TRISAbits.TRISA0=1;
	lcdint();
	lcdstring(str1);
	lcdcmd(0xc0);
	lcdstring(str2);
	while(1)
	{
			ADCON0=0x01;
			ADCON1=0x0e;
			ADCON2=0xCE;
			adc();
	}
}
void adc()
{	lcdcmd(0xca);
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