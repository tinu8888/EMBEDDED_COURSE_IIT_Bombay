#include <p18f4550.h>
#include<stdio.h>
#include "lcd.h"
unsigned char min[10];
unsigned char high_nibble,low_nibble,sec1,sec2,min1,min2,hour1,hour2;
int rval=0;
void I2C_Init()
{
    TRISBbits.TRISB0=1;		//setting as input
    TRISBbits.TRISB1=1;
	TRISBbits.TRISB2=0;		//setting as input
    TRISBbits.TRISB3=0;
	
    SSPSTAT=0X80;		/* Slew rate disabled for standard mode of operation(100khz), other bits are cleared */
    SSPCON1=0x28;	/* Enabling SSPEN and Master Mode clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPCON2=0x00;       /* To monitor Acknowledge/ Start/ Stop/ Repeated Start condition */
    SSPADD=0x31;	/* Clock 100 kHz */  
   
    PIE1bits.SSPIE=1;		/* Enable SSPIF interrupt */
    PIR1bits.SSPIF=0;
}
void BCD_ASCII(unsigned char value)   // Since we read from DS1307 and display it to LCD (which accepts only ASCII Value)
{
	high_nibble=value&0xF0;       // extracting higher nibble
	low_nibble=value&0x0F;	      // extracting lower nibble
	
	low_nibble|=0x30;	      // adding 0x30 to the number to make it ASCII representable (eg. : 5 in ASCII is 0X35)
	high_nibble=(high_nibble>>4); // shifting right the number by 4 bit positions 
	high_nibble|=0x30;
	
}
void BCDtoDecimal (char val)
{
    char res;
    res = (val & 0x0F) + ((val & 0xF0)>>4)*10;
    return res;
}
void write(unsigned char wval)           ///////////////// Master Transmit Mode////////////////////////
{
	SSPBUF=wval;                     // write value to SSPBUF that you want to transmit
	while(!PIR1bits.SSPIF);		 // The data will be sent automatically, you just monitor when does the transmission gets complete
	PIR1bits.SSPIF=0;                // Monitor SSPIF flag (Refer Timing Diagram to see Importance of this multipurpose Flag)
	
	if(SSPCON2bits.ACKSTAT==1)          // Keep record of Acknowledge sent "by Slave" Device
	{
		SSPCON2bits.PEN=1;       // If no acknowledge is received then Initiate Stop condition
		while(SSPCON2bits.PEN);	
	}	
}
void start(void)
{
	SSPCON2bits.SEN=1;             // To initiate Start Condition using SEN bit
	while(!PIR1bits.SSPIF);        // Monitor SSPIF flag (Refer Timing Diagram to see Importance of this multipurpose Flag)
	PIR1bits.SSPIF=0;
}

/* This part of code stops the communication by the Master, Master send "Stop bit" */
void stop(void)
{
	
	  SSPCON2bits.PEN=1;     
	while(SSPCON2bits.PEN);
}
void rep_start(void)
{	
	SSPCON2bits.RSEN=1;                       
	while(SSPCON2bits.RSEN);   
	PIR1bits.SSPIF=0;
	
}
void wait(void)
{	int WAit;
	WAit=0;
	while((WAit++)<10);
}
void write_time(unsigned char sec,unsigned char min,unsigned char hour)
{	
	wait();                 // Just to provide Delay before Start ( It might be possible that a stop condition is encountered before 					// calling this function, there must be some delay between a Start and Stop )
	start();
	write(0xD0);		// Find out the Slave address of DS1307 + write mode (last bit is '0')
	write(0x00);		// Find location of register storing "Seconds" value
	
	write(sec);
	write(min);
	write(hour);
	stop();
}
void write_date(unsigned char day,unsigned char date,unsigned char month,unsigned char year)
{
	wait();
	start();
	write(0xD0);		// Find out Slave address of DS1307 + write mode
	write(0x03);		// Find location of register storing "Days" value

	write(day);
	write(date);
	write(month);
	write(year);		// Configuring Square Wave Output for 1Hz 
	 
	stop();
}
void read(void)                            /////////////////// Master Receive Mode////////////////////////////////
{
	SSPCON2bits.RCEN=1;                // Configures I2C in Receive mode 

	while(!PIR1bits.SSPIF);		    // Refer Data-sheet 
	PIR1bits.SSPIF=0;
	rval=SSPBUF;
	rval=35;	   						// It holds received/transmitted value
	sprintf(min,"%x",rval);
	lcdstring(min);
	
}
void Nack(void)
{
	SSPCON2bits.ACKDT=1;               // value of ACKDT decides Acknowledge or Not-Acknowledge that will be transmitted 
	SSPCON2bits.ACKEN=1;		   // ACKEN transmits value ACKDT over I2C Bus. 
					   // This ACK/NAck values are sent by MASTER when it is receiving data from slave
	while(SSPCON2bits.ACKEN);

}
void ack(void)
{
	SSPCON2bits.ACKDT=0;               // value of ACKDT decides Acknowledge or Not-Acknowledge that will be transmitted 
	SSPCON2bits.ACKEN=1;		   // ACKEN transmits value ACKDT over I2C Bus. 
					   // This ACK/NAck values are sent by MASTER when it is receiving data from slave
	while(SSPCON2bits.ACKEN);

}
void read_time(void)
{	int a;	
		wait();		
		start();
		write(0xD0);		// Slave address + write mode enabled, Eighth bit = 0, Indicates Master wants to Write to Slave.
		write(0X00);        // To Read  from the slave, we first need to write to it the location from where to read.
		rep_start();
		write(0xD1);		// Slave address + read mode enabled, Eight bit = 1, Indicates Master wants to Read from Slave.
		read();
//		BCD_ASCII(rval);
	/*	if(a=0)
		{
			sec1=high_nibble;
			sec2=low_nibble;
		}
		if(a=1)
		{
			min1=high_nibble;
			min2=low_nibble;
		}
		if(a=2)
		{
			hour1=high_nibble;
			hour2=low_nibble;
		}
		Nack();
		stop();
	ptr++;
	}*/
	Nack();
		stop();
}

void main()
{	char str[]="time";
	lcdint();
	OSCCON=0x72;
	I2C_Init();
	write_time(0x02,0x47,0x06);//for AM
	write_date(0x01,0x27,0x06,0x18);

	lcdcmd(0x80);
	read_time();
//	lcdstring(str);
/*	lcddata(hour1);
	lcddata(hour1);
*///	lcddata(':');
/*	lcddata(min1);
	lcddata(min2);
	lcddata(':');
	lcddata(sec1);
	lcddata(sec2);
	}
*/
while(1);
}
