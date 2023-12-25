// 	Q1) Write a C-program which will store 10 different numbers in 10 consecutive memory locations.
//	Then clear these memory locations (i.e. make the contents of these memory locations to zero).
//	You should use an array for storing numbers in memory locations and use a pointer to an array
//	for clearing the content of the memory locations.

#include <p18f4550.h>
#pragma idata array=0x60				// store data from memory location 60H
int array[10]={0,1,2,3,4,5,6,7,8,9};			// array of length 10
void main()
{
	int *p;								// declare pointer variable	
	int i;	
	p=array;							//loading the address of first element of an array
	for(i=0;i<10;i++)
{	*p=0;								// making the pointer variable to zero
	p++; 								//increament the pointer
}
}