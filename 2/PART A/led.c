//						Lab Exercise-2: Part-A
//	Aim: To read the switch status and to accordingly display the status on the corresponding LED.
#include<P18f4550.h>
#include "boot.h" 			//interrupt vector header file.
#define sw1 PORTBbits.RB4
#define sw2 PORTBbits.RB5
#define sw3 PORTBbits.RB6
#define sw4 PORTBbits.RB7
#define led1 PORTBbits.RB0
#define led2 PORTBbits.RB1
#define led3 PORTBbits.RB2
#define led4 PORTBbits.RB3
void main()
{
	TRISB=0xf0;		//first 4bit as LED(output) and another 4bit Switch(Input)
	while(1)
	{
		if(sw1==1)
		{
		led1=1;			//led1 will on if switch1 is pressed
		}
		if(sw2==1)
		{
		led2=1;			//led2 will on if switch2 is pressed
		}
		if(sw3==1)
		{
		led3=1;			//led3 will on if switch3 is pressed
		}
		if(sw4==1)
		{
		led4=1;			//led4 will on if switch4 is pressed
		}
	else
	{
		PORTB=0;
	}
	}
	
}