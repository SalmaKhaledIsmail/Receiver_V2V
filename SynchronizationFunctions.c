/*
 * SynchronizationFunctions.c
 *
 *  Created on: Mar 27, 2017
 *      Author: DotCom
 */

#include "SynchronizationFunctions.h"

//---------------------------------------FUNCTIONS IMPLEMENTATIONS---------------------------------//
void movingAverage(float* arr,float* out ,short int size ,short int length,float*sum,float x[])
{

	short int i,lower,j;
	lower=0;
	j=0;

	for(i=0;i<size;i++)
	{
		// lower is lower edge of the window		i is the upper edge of the window
		if( i>length-1)
		{
			*sum+=arr[i]-arr[lower];
			lower++;
		}
		else
		{
			*sum+=arr[i] - x[j];
			j++;
		}

		out[i]=*sum;

	}

	memcpy(x,arr+size-length,length*sizeof(float));
}
//---------------------------------------------------------------------------
void movingAverageComplex(float complex* arr,float complex* out,short int size ,short int length,float complex *sum,float complex x1[] )
{

	short int i,lower;
	short int j;
	float x,y;
	lower=0;
	j=0;

	for(i=0;i<size;i++)
	{
		// lower is lower edge of the window		i is the upper edge of the window
		if( i>length-1)
		{
			x=crealf(arr[i])-crealf(arr[lower])+crealf(*sum);
			y=cimagf(arr[i])-cimagf(arr[lower])+cimagf(*sum);
			*sum = x+y*I;
			lower++;
		}
		else
		{
			x=crealf(arr[i])-crealf(x1[j])+crealf(*sum);
			y=cimagf(arr[i])-cimagf(x1[j])+cimagf(*sum);
			*sum = x+y*I;
			j++;
		}

		out[i]=*sum;

	}

	memcpy(x1,arr+size-length,length*sizeof(float complex));
}

//----------------------------------------------------------------------------

void complex2mag(float complex* arr,float* out ,short int size)
{
		short int i;

		for(i=0;i<size;i++)
		{
			out[i] = cabsf(arr[i]);

		}
}
//--------------------------------------------------------------------------

void complex2magSquare(const float complex* arr ,float* out,short int size )
{
	short int i;

	for(i=0;i<size;i++)
	{
		out[i] = pow(cabsf(arr[i]),2);

	}
}

//-------------------------------------------------------------------------
void delayy(const float complex *input,float complex *output,short int delay ,short int input_size, float complex delay_before_sync_short[])
{
	short int i=0;
	for (i=0;i<delay;i++)
	{
		output[i]=delay_before_sync_short[i];
	}

	for(i=delay;i<delay+input_size;i++)
	{
		output[i]=input[i-delay];
	}
}

//--------------------------------------------------------------------------
