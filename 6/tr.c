/*
NAME:VIVEK TIWARI
Q)In this experiment we will interface DAC 0808 with PIC18F4550 and will generate
sawtooth wave of 3.3V and frequency of 1KHz at the output of DAC
*/
#include<P18f4550.h>
#include "BOOT.H"	//isr 
int A;
void T0_isr1(void);
#pragma interrupt chk_isr	//used for high priority
void chk_isr(void)
{
	T0_isr1();	//call timer function
}
#pragma code
void main()
{	INTCON2bits.RBPU=0; // Enabling the weak internal pull-ups on PORTB
	TRISB=0X00;	//PORTB as output port
	T0CON=0X08;	//timer0 as 16-bit mode
	TMR0H=0Xff;	//1ms/64steps i.e 
	TMR0L=0Xb2;	//delay for 15.625usec
	INTCONbits.TMR0IF=0;//clear  TF0
	INTCONbits.TMR0IE=1;//enable timer0 interrupt
	INTCONbits.GIE=1;// enabling global interrupt
	T0CONbits.TMR0ON=1;//TURN ON TIMER0
	while(1)
	{
		PORTB=A;	//transmit value to PORTB
	}
}
void T0_isr1(void)
{		A=A+4;		//increment the value by 4 if isr occur
		if(A>255)	//if value reached 255 than again reset the A
		{
			A=0;
	}
    TMR0H=0Xff;	//1ms/64steps i.e 
	TMR0L=0Xb2;	//delay for 15.625usec
	INTCONbits.TMR0IF=0;//CLEAR TF0
}	
