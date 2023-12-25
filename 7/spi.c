#include<P18f4550.h>
#include "boot.h"		//ISR
#define CS PORTBbits.RB2 
#define LDC PORTBbits.RB3
void spi(unsigned char data)
{
	SSPBUF=data;			//copy data to SSPBUF
	while(!SSPSTATbits.BF);	//wait 
}
void main()
{	INTCON2bits.RBPU=0;
	TRISBbits.TRISB3=0;	//LDC
	TRISBbits.TRISB2=0;	//CS
	TRISBbits.TRISB1=0;	//SCK
	TRISCbits.TRISC7=0;	//SDO
	SSPSTAT=0X40;		
	SSPCON1=0XA0;
	
	while(1)
{	CS=0;
	LDC=1;
	spi(0x38);
	spi(0x00);
	CS=1;
	LDC=0;
}

}