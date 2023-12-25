#include <p18f4550.h>
#include "lcd.h"
unsigned char sec,min,hour,date,month,year;
const unsigned char time[] = "TIME:";
const unsigned char date_format[] = "DATE:";
const unsigned char rtc[] = "RTC DS1307";
int point_add,clk,j,PM,AM;
unsigned char clock[10],high_nibble,low_nibble,rval,check;

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
void read(void) 
{
	SSPCON2bits.RCEN=1;                // Configures I2C in Receive mode 

	while(!PIR1bits.SSPIF);		    // Refer Data-sheet 
	PIR1bits.SSPIF=0;
	rval=SSPBUF;			   // It holds received/transmitted value
	
}
void Nack(void)
{
	SSPCON2bits.ACKDT=1;               // value of ACKDT decides Acknowledge or Not-Acknowledge that will be transmitted 
	SSPCON2bits.ACKEN=1;		   // ACKEN transmits value ACKDT over I2C Bus. 
					   // This ACK/NAck values are sent by MASTER when it is receiving data from slave
	while(SSPCON2bits.ACKEN);

}
void Ack(void)
{ 	
SSPCON2bits.ACKDT=0;               // value of ACKDT decides Acknowledge or Not-Acknowledge that will be transmitted 
	SSPCON2bits.ACKEN=1;		   // ACKEN transmits value ACKDT over I2C Bus. 
					   // This ACK/NAck values are sent by MASTER when it is receiving data from slave
	while(SSPCON2bits.ACKEN);	
}
void rep_start(void)
{
	SSPCON2bits.RSEN=1;                       
	while(SSPCON2bits.RSEN);   
	PIR1bits.SSPIF=0;
}
void I2C_Init(void)
{
 TRISBbits.TRISB0=1;		//setting as input
 TRISBbits.TRISB1=1;	
    SSPSTAT=0X80;		/* Slew rate disabled for standard mode of operation(100khz), other bits are cleared */
    SSPCON1=0x28;	/* Enabling SSPEN and Master Mode clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPCON2=0x00;       /* To monitor Acknowledge/ Start/ Stop/ Repeated Start condition */
    SSPADD=0x31;	/* Clock 100 kHz */  
   
    PIE1bits.SSPIE=1;		/* Enable SSPIF interrupt */
    PIR1bits.SSPIF=0;
}
void wait(void)
{	int WAit;
	WAit=0;
	while((WAit++)<10);
}
unsigned char BCD2UpperCh(unsigned char bcd)
{
	unsigned char temp;
 	temp = bcd >> 4;
 	temp = temp | 0x30;
 	return(temp);	
}
unsigned char BCD2LowerCh(unsigned char bcd)
{
	unsigned char temp;
 	temp = bcd & 0x0F; //Making the Upper 4-bits
 	temp = temp | 0x30;
 	return(temp);
}
void BCD_ASCII(unsigned char value)   // Since we read from DS1307 and display it to LCD (which accepts only ASCII Value)
{
	high_nibble=value&0xF0;       // extracting higher nibble
	low_nibble=value&0x0F;	      // extracting lower nibble
	low_nibble|=0x30;	      // adding 0x30 to the number to make it ASCII representable (eg. : 5 in ASCII is 0X35)
	high_nibble=(high_nibble>>4); // shifting right the number by 4 bit positions 
	high_nibble|=0x30;
	
}
void I2C_Wait(void)
{
	while(SSPSTATbits.R== 1)
		continue;
	if(SSPCON2bits.ACKSTAT == 1)
	{
		stop();
	}
	//If ACKSTAT bit is 0 Acknowledgment Received Successfully
	//Otherwise Not
}
unsigned char I2C_Read(void)
{
	unsigned char temp;
    SSPCON2bits.RCEN = 1;        /* Enable data reception */
    while(SSPSTATbits.BF == 0)      /* wait for buffer full */
    	continue;
    temp = SSPBUF;   /* Read serial buffer and store in temp register */
    I2C_Wait();       /* wait to check any pending transfer */
    SSPCON2bits.ACKDT=1;				//send not acknowledge
	SSPCON2bits.ACKEN=1;
	while(SSPCON2bits.ACKEN == 1) 
		continue;
	//I2C_Stop();
    return temp;     /* Return the read data from bus */
}
unsigned char rtc1307_read(unsigned char address)
{
	unsigned char temp;
	start();
	write(0xD0);
	write(address);
	rep_start();
	write(0xD1);
	temp = I2C_Read();
	stop();	//this line is Removed Because it is already included in I2C_read function
	return temp;
}
void main()
{
	lcdint();
	OSCCON|=0x72;      
	I2C_Init();
	wait();      
	start();
	write(0XD0);		// Find out the Slave address of DS1307 + write mode (last bit is '0')
	write(0X00);
	write(0x00);		// Find location of register storing "Seconds" value
	write(0x53);
	write(0x08);
	write(0X02);		// Find out the Slave address of DS1307 + write mode (last bit is '0')
	write(0X29);
	write(0x10);		// Find location of register storing "Seconds" value
	write(0x12);	
	stop();
	while(1)
	{
		sec = rtc1307_read(0x00);
		
		min = rtc1307_read(0x01);
		
		hour = rtc1307_read(0x02);
		
		date = rtc1307_read(0x04);
		
		month = rtc1307_read(0x05);
		
		year = rtc1307_read(0x06);
		lcd_cmd(0x80);
		lcd_data('t');
		BCD_ASCII(hour);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		BCD_ASCII(min);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		BCD_ASCII(sec);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		BCD_ASCII(date);
		lcd_cmd(0xc0);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		BCD_ASCII(month);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		BCD_ASCII(year);
		lcd_data(':');
		lcd_data(high_nibble);
		lcd_data(low_nibble);
		delay(1);
		/*lcd_data(BCD2UpperCh(min));
		lcd_data(BCD2LowerCh(min));
		lcd_data(':');
		lcd_data(BCD2UpperCh(sec));
		lcd_data(BCD2LowerCh(sec));
		lcd_cmd(0xC0);
		lcd_data('d');
		lcd_data(BCD2UpperCh(date));
		lcd_data(BCD2LowerCh(date));
		lcd_data('/');
		lcd_data(BCD2UpperCh(month));
		lcd_data(BCD2LowerCh(month));
		lcd_data('/');
		lcd_data(BCD2UpperCh(year));
		lcd_data(BCD2LowerCh(year));		
		delay(10);
*/
	}
}

