//1. Connect the LCD display to the Aurum Kit and write a program which will display two lines of
//text on the display. The lines are stored in the form of null terminated strings. Use today’s date
//(for example Wed Feb 14 2018) for the first line and your name for the second line. Display the
//two lines so that they are left aligned.

#include <p18f4550.h>
#include "boot.h"	//Bootloader
#include "lcd.h"	//LCD Header Files
void main()
{	char str1[] = "NIKHIL BOMBARDE";
	char str2[] = "313073";
	lcdint();
	lcd_string(str1);// Print First line
	lcd_cmd(0xc0);
	lcd_string(str2);//Print second line
	while(1);
}
