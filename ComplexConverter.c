/*
 * ComplexConverter.c
 *
 *  Created on: Mar 26, 2017
 *      Author: DotCom
 */

#include "ComplexConverter.h"

//---------------------------------------FUNCTIONS IMPLEMENTATIONS---------------------------------//
void ToComplex(float *realimagarray, float complex *ComplexArray, int realimagsize)
{
	//realimagarray is the array[2n] as real part and imaginary part are disconnected
	//ComplexArray is the output complex type array (x + I*y)
	//realimagsize is the size of realimagarray

	int i=0;	//real part indices (0,2,4,6,8...)
	int j=1;	//imaginary part indices (1,3,5,7,9...)
	int k=0;	//Complex array counter (half of input array)

	float realpart = 0;
	float imagpart = 0;

	for(k=0;k<=(realimagsize/2);k++)
	{
		realpart = realimagarray[i];
		imagpart = realimagarray[j];

		ComplexArray[k] = realpart + imagpart * I;
		i+=2;
		j+=2;
	}
}

void SeperateComplex(float complex *ComplexArray,float *realimagarray,int size)
{
	float realpart = 0;
	float imagpart = 0;
	int counter = 0; 	//complex array counter
	int counter2 = 0;	//Separated array counter

	for(counter =0;counter<size;counter++)
	{
		realpart = creal(ComplexArray[counter]);
		imagpart = cimag(ComplexArray[counter]);
		realimagarray[counter2] = realpart;
		counter2++;
		realimagarray[counter2] = imagpart;
		counter2++;
	}
}
