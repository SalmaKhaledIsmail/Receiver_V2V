/*
 * SynchronizationFunctions.h
 *
 *  Created on: Mar 27, 2017
 *      Author: DotCom
 */

#ifndef SYNCHRONIZATIONFUNCTIONS_H_
#define SYNCHRONIZATIONFUNCTIONS_H_

#include "complex.h"
#include <stdlib.h>
#include <math.h>

//--------------------------------------------FUNCTIONS DECLRATIONS-------------------------------------//
void movingAverage(float* arr,float* out ,short int size ,short int length,float*sum,float x[]);

void movingAverageComplex(float complex* arr,float complex* out,short int size ,short int length,float complex *sum,float complex x1[] );


void complex2mag(float complex* arr ,float* out,short int size);

void complex2magSquare(const float complex* arr,float* out ,short int size  );

void delayy(const float complex *input,float complex *output,short int delay ,short int input_size, float complex delay_before_sync_short[]);

#endif /* SYNCHRONIZATIONFUNCTIONS_H_ */
