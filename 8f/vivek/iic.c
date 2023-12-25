#include <p18f4550.h>
#include "lcd.h" //LCD HEADER FILE
#include "boot.h" //ISR SUBROUTINE
#include "i2c.h" //I2C HEADER FILE
#pragma code
void _isr1(void);
#pragma interrupt chk_isr
void chk_isr(void)
{
	_isr1(); // EXECUTE INT2 PROGRAM
}
#pragma code
void readrtc();
void main()
{	TRISBbits.TRISB2=1;	//INT20=INPUT
	INTCON3bits.INT2IF=0;// CLEAR FLAG
	INTCON3bits.INT2IE=1; //ENABLE  INTERRUPT
	INTCONbits.GIE=1;// ENABLE ALL INTERRUPT 
	LCD_Init();	//LCD INTIALIZE
	I2C_Init();//I2C INIIALIZE
	I2C_Start();//START I2C	
	I2C_Send(0xD0);//DEVICE ID + WRITE MODE
	I2C_Send(0x00);//STARTING ADDRESS
	I2C_write_time(0x00,0x09,0x09);//TIME (SEC,MIN,HOUR)
	I2C_write_date(0x00,0x01,0x07,0x18,0x90);//DATE (DAY,DATE,MONTH,YEAR,SQWE)
	I2C_Stop();//STOP I2C
	LCD_Cmd(0x80);
	LCD_Str(time);
	LCD_Cmd(0xC0);
	LCD_Str(date_format);	
	while(1);
}
void _isr1(void)
{
	readrtc();//READ TIME/DATE FROM DS1307 &DISPLAY
	INTCON3bits.INT2IF=0;// CLEAR FLAG
}	
void readrtc()
{	
	sec = rtc_read(0x00);//SEC READ LOCATION
	min = rtc_read(0x01);//MIN READ LOCAION
	hour = rtc_read(0x02);//HOUR READ LOCATION
	day = rtc_read(0x03);//DAY READ LOCATION
	date = rtc_read(0x04);//DATE READ LOCATION
	month = rtc_read(0x05);//MONTH READ LOCATION
	year = rtc_read(0x06);//YEAR READ LOCATION
		LCD_Cmd(0x85);
		LCD_Data(BCD2UpperCh(hour));
		LCD_Data(BCD2LowerCh(hour));
		LCD_Data(':');
		LCD_Data(BCD2UpperCh(min));
		LCD_Data(BCD2LowerCh(min));
		LCD_Data(':');
		LCD_Data(BCD2UpperCh(sec));
		LCD_Data(BCD2LowerCh(sec));
		LCD_Cmd(0xc5);
		LCD_Data(BCD2UpperCh(date));
		LCD_Data(BCD2LowerCh(date));
		LCD_Data('/');
		LCD_Data(BCD2UpperCh(month));
		LCD_Data(BCD2LowerCh(month));
		LCD_Data('/');
		LCD_Data(BCD2UpperCh(year));
		LCD_Data(BCD2LowerCh(year));
		LCD_Data(' ');
		day1=BCD2LowerCh(day);//SELECT PROPER DAY
		if(day1=='0')
		{
			LCD_Str(str0);
		}
		if(day1=='1')
		{
			LCD_Str(str1);
		}
		if(day1=='2')
		{
			LCD_Str(str2);
		}
		if(day1=='3')
		{
			LCD_Str(str3);
		}
		if(day1=='4')
		{
			LCD_Str(str4);
		}
		if(day1=='5')
		{
			LCD_Str(str5);
		}
		if(day1=='6')
		{
			LCD_Str(str6);
		}


}