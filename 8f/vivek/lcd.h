#define LCD_DATA_PINS    PORTD              
#define en          PORTEbits.RE2     
#define rw          PORTEbits.RE1     
#define rs          PORTEbits.RE0     
void LCD_Cmd(unsigned char value);
void LCD_Data(unsigned char value);
void LCD_Str(static char *str);
void LCD_Init();
void delay(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<500;j++);
}

void LCD_Cmd(unsigned char value)
	{	delay(15);
		LCD_DATA_PINS=value;
		rs=0;
		rw=0;
		en=1;
		delay(15);
		en=0;
		

	}
void LCD_Data(unsigned char value)
	{	delay(15);
		LCD_DATA_PINS=value;
		rs=1;
		rw=0;
		en=1;
		delay(15);
		en=0;

	}

void LCD_Str(static char *str)
{
    int i = 0;
    while (str[i] != 0)
    {
        LCD_Data(str[i]);     
        delay(15);
                i++;
    }
    return;
}
void LCD_Init()
{	ADCON1=0x0E;	
	TRISD=0x00;
	TRISE=0x00;
	LCD_Cmd(0x38);
	LCD_Cmd(0x0e);
	LCD_Cmd(0x01);
	LCD_Cmd(0x06);
	LCD_Cmd(0x80);
}
