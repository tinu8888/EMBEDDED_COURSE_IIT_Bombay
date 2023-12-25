#include <p18f4550.h>
#include "lcd.h"
int point_add,clk,j,PM,AM;
unsigned char clock[10],high_nibble,high_nibble,low_nibble,rval,check;
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
	wait();      
	start();
	write(0XD0);		// Find out the Slave address of DS1307 + write mode (last bit is '0')
	write(0X00);
	write(sec);		// Find location of register storing "Seconds" value
	write(min);
	write(hour);
	
	stop();
}
void write_date(unsigned char day,unsigned char date,unsigned char month,unsigned char year,unsigned char SQWE)
{
	wait();
	start();
	write(0XD0);		// Find out Slave address of DS1307 + write mode
	write(0x03);		// Find location of register storing "Days" value

	write(day);
	write(date);
	write(month);
	write(year);
	write(SQWE);		// Configuring Square Wave Output for 1Hz 
	 
	stop();
}
void read(void)                            /////////////////// Master Receive Mode////////////////////////////////
{
	SSPCON2bits.RCEN=1;                // Configures I2C in Receive mode 

	while(!PIR1bits.SSPIF);		    // Refer Data-sheet 
	PIR1bits.SSPIF=0;
	rval=SSPBUF;
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
{	int j;
	point_add=0x00;			// Reset value of pointer corresponding to Location of "Sec" Register
	j=0;
	while(j<3)                      // We are reading one register value at a time and then stopping the communication 
	{				////* We can also read continuously from Sec,Min,Hour register and then stop the communication*////
		wait();		
		start();
		write(0XD0);		// Slave address + write mode enabled, Eighth bit = 0, Indicates Master wants to Write to Slave.
		write(point_add);        // To Read  from the slave, we first need to write to it the location from where to read.
		rep_start();
		write(0XD1);		// Slave address + read mode enabled, Eight bit = 1, Indicates Master wants to Read from Slave.

	
		read();
		Nack();
		stop();
		clock[j]=rval;
		point_add++;
		j++;
	}
}
void read_date(void)
{	
	point_add=0x03;			// Reset value of pointer corresponding to Location of "Days" Register
	j=3;
	while(j<=7)                     // We are reading one register at a time and then stopping the communication
	{				// followed by again starting the communication and reading the value of next register
					// at location "point_add"
					
					//* we could have continually read Day,Date,Month,Year,SQWE and then stopped communication once Only*//
					//* In the continuous read case the Slave pointer automatically increments to next location*//
		wait();		
		start();
		write(0XD0);     	// Slave address + write mode
		write(point_add);	// Reset the pointer to location from where we want to read
		rep_start();
		write(0XD1);		// Slave address + read mode
		read();
		Nack();
		stop();
		clock[j]=rval;
		point_add++;
		j++;
	}
}
void Disp_time(void)
{
		
		lcd_cmd(0x83);                     // Relocation LCD pointer to the location where we intend to print TIME.
		for(clk=2;clk>=0;clk--)
		{
			
			
			BCD_ASCII(clock[clk]);
			if(clk==2)
			{
				check=high_nibble&0x06;     
				switch (check)
				{
					case 0x06 :
								PM=1;
								AM=0;
										high_nibble=(high_nibble&0x01)|0x30;
								break;
					default  :
								high_nibble=(high_nibble&0x03)|0x30;
								
				}
			}

			lcd_data(high_nibble);
			lcd_data(low_nibble);
			lcd_data(' ');
		}

	if(PM==1)
	{
		lcd_data('P');

	}
	else if(AM==1)
	{
		lcd_data('A');
	}
	else
	{	
		lcd_data('H');
	}

}
void Disp_date(void)
{
		lcd_cmd(0xc3);              

		for(clk=3;clk<7;clk++)
		{
			
			
			BCD_ASCII(clock[clk]);
		
			if(clk==3)
			{
				check=low_nibble&0x0F;       // Refer data-sheet of DS1307 and see the "Date" registers format.  
				switch (check)
				{
							case 0x01 :
								lcd_data('M');   
								break;
							case 0x02 :
								lcd_data('T');    
								break;
case 0x03 :
								lcd_data('W');  
								break;
							case 0x04 :
								lcd_data('T');     
								break;
							case 0x05 :
								lcd_data('F');  
								break;
							case 0x06 :
								lcd_data('S'); 
								break;
					default   :
								lcd_data('S');
								
				}
			}
			
		else
			{

			lcd_data(high_nibble);
			lcd_data(low_nibble);
			lcd_data('/');
			}
		}
}
void main()
{	

	lcdint();
	OSCCON|=0x72;            // Refer Data-sheet
		
	I2C_Init();


	write_time(0x00,0x20,0x10);//for AM
			write_date(0x04,0x28,0x06,0x18,0x00); 
		
	while(1)
{
read_time();
		read_date();
	lcd_cmd(0x80);
lcd_data('T');
lcd_data(':');	
		Disp_time();
		lcd_cmd(0xC0);
		lcd_data('D');
lcd_data(':');
		Disp_date();
}
	
}
