#include <P18f4550.h>
#include "boot.h"	//Bootloader
void delay()
{
	int i,j;
	for(i=0;i<10;i++)
	for(j=0;j<100;j++);
}
void serTx(unsigned char msg)//serial transmit
{ 	delay();
	TXREG=msg;		//	send the character in TXREGregister.	
   	while(PIR1bits.TXIF==0);// wait for TXIF flag               
}
void txstring(static char *str)// send string serially
{

    while (*str)			
    {
        serTx(*str);
                str++;
    }
    return;
}
void uartint()
{
   SPBRG=77;				//9600 baud rate at FOSC=48Mhz	
   TRISCbits.TRISC6=0;		//tx pin as output	
   TXSTAbits.TXEN= 1;		//Enable the transmission	
   RCSTAbits.SPEN=1;		//Enable Serial Port for communication	
   RCSTAbits.CREN=1;		//Enable receiver for continuous reception of data byte

}
void main(void)
{	char str[] = " VIVEK TIWARI ";
	 uartint();				//uart initialize
	txstring(str);			//string transmit
	while(1);

}