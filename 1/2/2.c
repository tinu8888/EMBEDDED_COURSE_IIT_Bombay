//2. Write a C-program to find the sum of N numbers starting from 1 and store the partial sum
//values in N consecutive memory locations. Your program should result in having 1, 3, and 6 in
//initial memory locations. The value of N should be taken between10-20. You should use an array
//for storing numbers in memory locations and use a pointer to an array for accessing the content
//of previous memory location.
#include <p18f4550.h>
#pragma idata array=0x60		// store data from memory location 60H
int array[20];					//array of length 20
void main()
{
	int *p;										// declare pointer variable	
	int i,j,n;
	n=20; 										//Number=20	
	p=array;
	for(i=1;i<n;i++)							//loading the address of first element of an array
	{
	j=*p;										//last variable 
	array[i]=j+i;								//adding value and loading
	p++;										//increment pointer
	}

}