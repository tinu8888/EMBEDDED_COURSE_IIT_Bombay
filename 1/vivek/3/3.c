//Q3)Write a C-program which will read 10 numbers from one set of locations and copy these
//numbers to a different set of locations. You should use an array for storing numbers in memory
//locations and use a pointer to an array for accessing the contents of memory locations.
#include <p18f4550.h>
#pragma idata array1=0x60 // 1st array location from 60H	
int array1[10]={0,1,2,3,4,5,6,7,8,9};	//1st array data
int array2[10];// 2nd array where we have to copy the data of 1st array
void main()
{
	int *p;	//pointer vairable								
	int i,j; 									
	p=array1;
	for(i=0;i<10;i++)						
	{
	j=*p;			//copying data from 1st array;						
	array2[i]=j;	//storing to array2						
	p++;			//increament pointer						
	}

}