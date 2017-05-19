/*
 * syncshort.h
 *
 *  Created on: Mar 26, 2017
 *      Author: DotCom
 */

#ifndef SYNCSHORT_H_
#define SYNCSHORT_H_

#include "complex.h"
#include "SynchronizationFunctions.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN_GAP 480
#define MAX_SAMPLES 43200
#define d_threshold 0.560f
#define min_plateau 2
typedef enum {
	SEARCH=0,
	COPY=1,
}enum_state;

void syncShort(enum_state* d_state, float complex* in, float complex* in_abs, float* in_cor, float complex* out,short int InputSize,short int* OutSize, unsigned char* d_plateau,int * d_copied,int ReachMinGap [], int* IndexReachMinGap,float* d_freq_offset,FILE *fp);
void functionsBeforeSyncShort(float complex* RxInput, short int RxInputSize,float complex* Delay16Output,float complex delay_before_sync_short[],float complex* MovAvgCompOutput,float* DivideOutput, float* sum_MovingAverage, float complex* sum_MovingAverage_Complex, float Ref_For_MovingAvergae[], float complex Ref_For_MovingAvergae_Complex[]);
#endif /* SYNCSHORT_H_ */
