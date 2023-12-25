#define lcd    PORTD              
#define en          PORTEbits.RE2     
#define rw          PORTEbits.RE1     
#define rs          PORTEbits.RE0     
void lcdcmd(unsigned char value);
void lcddata(unsigned char value);
void lcdstring(static char *str);
void lcdint();
void delay(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<710;j++);
}

void lcdcmd(unsigned char value)
	{	delay(15);
		lcd=value;
		rs=0;
		rw=0;
		en=1;
		delay(15);
		en=0;
		

	}
void lcddata(unsigned char value)
	{	delay(15);
		lcd=value;
		rs=1;
		rw=0;
		en=1;
		delay(15);
		en=0;

	}

void lcdstring(static char *str)
{
    int i = 0;
    while (str[i] != 0)
    {
        lcddata(str[i]);      // sending data on LCD byte by byte
        delay(15);
                i++;
    }
    return;
}
void lcdint()
{	
	ADCON1=0x0E;	
	TRISD=0x00;
	TRISE=0x00;
	lcdcmd(0x38);
	lcdcmd(0x0e);
	lcdcmd(0x01);
	lcdcmd(0x06);
	lcdcmd(0x80);
}
