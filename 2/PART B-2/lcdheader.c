#include <p18f4550.h>
#include "boot.h" //bootloader
#include "lcd.h"	//lcd header file
void main()
{	
	char str1[] = "My first lab program works!";
	lcdint();
	lcdcmd(0x8D);	//enter from left side of display
	lcdstring(str1);
	while(1)
	{	
	lcdroated();	//roted lcd display towards left
	}	

}
