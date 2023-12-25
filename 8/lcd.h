#define lcd    PORTD              
#define en          PORTEbits.RE2     
#define rw          PORTEbits.RE1     
#define rs          PORTEbits.RE0     
void lcd_cmd(unsigned char value);
void lcd_data(unsigned char value);
void lcd_string(static char *str);
void lcdint();
void delay(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<710;j++);
}

void lcd_cmd(unsigned char value)
	{	delay(15);
		lcd=value;
		rs=0;
		rw=0;
		en=1;
		delay(15);
		en=0;
		

	}
void lcd_data(unsigned char value)
	{	delay(15);
		lcd=value;
		rs=1;
		rw=0;
		en=1;
		delay(15);
		en=0;

	}

void lcd_string(static char *str)
{
    int i = 0;
    while (str[i] != 0)
    {
        lcd_data(str[i]);     
        delay(15);
                i++;
    }
    return;
}
void lcdint()
{	ADCON1=0x0E;	
	TRISD=0x00;
	TRISE=0x00;
	lcd_cmd(0x38);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);
	lcd_cmd(0x06);
	lcd_cmd(0x80);
}
