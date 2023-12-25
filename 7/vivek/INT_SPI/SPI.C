#include<P18f4550.h>
#include "BOOT.H"
void isr(void);
#pragma interrupt chk_isr	//used for high priority
void chk_isr(void)
{
	isr();
}
#pragma code
void main()
{	INTCONbits.GIE=1;
	INTCONbits.PEIE=1;
	PIEbits.SSPIE=1;
	PIR1bits.SSPIF=0;
}
void isr(void)
{	
	LATBbits.LATB0=~LATBbits.LATB0;//TOGGLE PORTB.0
    TMR0H=0X67;
	TMR0L=0X69;
	INTCONbits.TMR0IF=0;//CLEAR TF0
}	
