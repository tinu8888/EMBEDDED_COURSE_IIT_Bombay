#include<P18f4550.h>
int  i;
void chk_isr(void);
void T0_isr1(void);
void pwm20(void);
#pragma interrupt chk_isr
void chk_isr(void)
{
	if (INTCONbits.TMR0IF==1)
	T0_isr1();
}	
//	T0_isr2();
#pragma code My_hipro_int=0x0008
void My_hipro_int(void)
{
	_asm
	GOTO chk_isr
	_endasm
}
#pragma code
void main()
{	INTCON2bits.RBPU=0;
	TRISB=0x00;	
	i=0;
//TRISB=0x00;
//	TRISBbits.TRISB2=0;
	T0CON=0X08;
	TMR0H=0XFF;
	TMR0L=0X38;
	INTCONbits.TMR0IF=0;
	INTCONbits.TMR0IE=1;
	INTCONbits.PEIE=1;
	INTCONbits.GIE=1;
	T0CONbits.TMR0ON=1;
	while(1)
	{
}
}
void T0_isr1(void)
{	pwm20();
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
void pwm20(void)
{		i++;
		if(i==5)
		{
			i=0;
		}
}