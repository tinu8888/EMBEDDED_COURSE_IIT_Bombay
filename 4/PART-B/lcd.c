
#include <p18f4550.h>
#include "boot.h"	//Bootloader
#include "lcd.h"	//LCD Header Files
void main()
{	char str1[] = "HELLO";
	lcdint();
	lcdstring(str1);// Print First line
	while(1);
}
