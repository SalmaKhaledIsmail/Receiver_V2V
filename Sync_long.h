/*
 * Sync_long.h
 *
 *  Created on: Apr 16, 2017
 *      Author: Samer
 */

#ifndef SYNC_LONG_H_
#define SYNC_LONG_H_


#include "ti/dsplib/dsplib.h"
# include "complex.h"
#include <stdlib.h>
#include <math.h>
#include "SynchronizationFunctions.h"
//#include "variables.h"
enum state{
			Sync,Copy,Reset
		};

struct d_corr{

	float complex value;
	int d_offset;
};

 ////////////////////////////////////////////////////////////////////////////////////////////

int min(int a, int b);

void search_frame_start(struct d_corr  cor[],int SYNC_length ,int *frame_start,float *freq_offset) ;


/////////////////////////////////////////////////////////////////////////////////////////////////

float complex polarToComplex( float mag, float phase );
struct d_corr* defineList(int size);

void ComplexToFloat(float complex* in ,int size,float*out);

void FloatToComplex (float*in , int size,float complex* out);

float complex * Sync_long_general_work(float complex* in, int ninput,short int* OutputSize);
#endif /* SYNC_LONG_H_ */
