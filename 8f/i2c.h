void I2C_Init(void);
void I2C_Start(void);
void I2C_Restart(void);
void I2C_Stop(void);
void I2C_Wait(void);
void I2C_Send(unsigned char dat);
void I2C_write_time(unsigned char sec,unsigned char min,unsigned char hour);
void I2C_write_date(unsigned char day,unsigned char date,unsigned char month,unsigned char year,unsigned char SQWE);
unsigned char I2C_Read(void);
unsigned char rtc_read(unsigned char address);
unsigned char BCD2UpperCh(unsigned char bcd);
unsigned char BCD2LowerCh(unsigned char bcd);
unsigned char sec,min,day,day1,hour,date,month,year;
const unsigned char time[] = "TIME:";
const unsigned char date_format[] = "DATE:";
char str0[]="Su";
char str1[]="Mo";
char str2[]="Tu";
char str3[]="We";
char str4[]="Th";
char str5[]="Fr";
char str6[]="Sa";
void __delay_ms(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<500;j++);
}
unsigned char BCD2UpperCh(unsigned char bcd) //BCD TO ASCII 1
{
	unsigned char temp;
 	temp = bcd >> 4;
 	temp = temp | 0x30;
 	return(temp);	
}
unsigned char BCD2LowerCh(unsigned char bcd) //BCD TO ASCII 2
{
	unsigned char temp;
 	temp = bcd & 0x0F;
 	temp = temp | 0x30;
 	return(temp);
}

unsigned char rtc_read(unsigned char address)	//READ FUNCTION
{
	unsigned char temp; 
	I2C_Start();
	I2C_Send(0xD0);		//DEVICE ID + WRITE MODE
	I2C_Send(address); //READ FROM GIVEN LOCATION
	I2C_Restart(); 		//RESTART
	I2C_Send(0xD1);//DEVICE ID + READ MODE
	temp = I2C_Read();//SAVE TO TEMP LOCATION
	I2C_Stop(); //STOP READING 
	return temp;
}

void I2C_Init()
{
	TRISBbits.TRISB0=1; //SDA = INPUT
	TRISBbits.TRISB1=1; //SCL = INPUT 
	SSPSTAT |= 0x80;	//1 = Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
	SSPADD = 0X31;		/* Clock 100 kHz */
	SSPCON1 = 0X28;		/* Enabling SSPEN and Master Mode clock = FOSC / (4 * (SSPADD+1))*/ 
}

void I2C_Start(void)
{
	SSPCON2bits.SEN = 1;	// To initiate Start Condition using SEN bit
	while (SSPCON2bits.SEN == 1);
}
/* When Master device wants to restart the communication, instead to sending STOP and then START, it can directing send Repeat_Start command to slave device by calling this function */
void I2C_Restart(void)
{
	SSPCON2bits.RSEN = 1;
	while (SSPCON2bits.RSEN == 1);
}

void I2C_Stop(void)
{
	SSPCON2bits.PEN=1; // To initiate Stop Condition using PEN bit
	while(SSPCON2bits.PEN==1);
}

void I2C_Wait(void)
{
	while(SSPSTATbits.R== 1);
	if(SSPCON2bits.ACKSTAT == 1)//WAIT FOR ACK
	{
		I2C_Stop();
	}
}

void I2C_Send(unsigned char dat)
{
	SSPBUF = dat; //SEND DATA TO BUFFER 
    while(SSPSTATbits.BF);  // MONITER THE FLAG    
    I2C_Wait();
}

unsigned char I2C_Read(void)
{
	unsigned char temp;
    SSPCON2bits.RCEN = 1;  //1 = Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware.    
    while(SSPSTATbits.BF == 0); // WAIT FOR DATA TO LOAD ON BUFFER
    temp = SSPBUF; //SAVE RECEIVED DATA TO TEMP
    I2C_Wait();
    SSPCON2bits.ACKDT=1; //SEND Not Acknowledge
	SSPCON2bits.ACKEN=1;	//1 = Initiate Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. Automatically cleared by hardware
	while(SSPCON2bits.ACKEN == 1);
    return temp;
}
void I2C_write_time(unsigned char sec,unsigned char min,unsigned char hour)
{
	
	I2C_Send(sec);	
	I2C_Send(min);	
	I2C_Send(hour); 
	
}
void I2C_write_date(unsigned char day,unsigned char date,unsigned char month,unsigned char year,unsigned char SQWE)
{
	I2C_Send(day);	
	I2C_Send(date);	
	I2C_Send(month);	
	I2C_Send(year);
	I2C_Send(SQWE);
	
}
