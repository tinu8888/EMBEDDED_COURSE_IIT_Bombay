//Q)		Write a C-program to sort 5 numbers (that are stored in some memory locations) in an
//		increasing order using any sorting algorithm. Store the sorted numbers in some other memory
//		locations. You should use an array for storing numbers in memory locations and use a pointer to
//		an array for accessing the content of memory locations. The contents of the original array should
//		not be destroyed or re-ordered.
#include <p18f4550.h>
#pragma idata array1=0x60 	// 1st array location from 60H	
int array1[5]={7,5,4,3,2};	//data to be sorted
int array2[5];				// 2nd array with sorted numbers
void main()							
{	int i,j,a,b,*p; 									
	p=array1;
	for(i=0;i<5;i++)						
	{
		j=*p;						//copying data from 1st array;						
		array2[i]=j;				//storing to array2						
		p++;						//increament pointer						
	}
	
	for(j=0;j<4;j++)
		{	p=array2; 				//ponting 2nd array
		for(i=0;i<4;i++)						
			{
				a=*p;
				p++;					//if data is in increasing order
				b=*p;
				if(a>b)
				{
					array2[i]=b;
					i++;				//swaping of data
					array2[i]=a;
					i--;
			
				}
			}
		}
}