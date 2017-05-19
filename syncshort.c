/*
 * syncshort.c
 *
 *  Created on: Mar 26, 2017
 *      Author: DotCom
 */

#include "syncshort.h"

void syncShort(enum_state* d_state, float complex* in, float complex* in_abs,
		float* in_cor, float complex* out, short int InputSize,
		short int* OutSize, unsigned char* d_plateau, int * d_copied,
		int ReachMinGap[], int* IndexReachMinGap,float* d_freq_offset,FILE *fp) {

	//variables declaration



	float argument;
	short int InputCounter = 0;
	short int OutputCounter = 0;
	short int i;
	*OutSize = InputSize;
	float complex ExpPower = 0 + 0 * I;

	//Read file loop
	while (InputCounter < InputSize) {

		switch (*d_state) {
		case SEARCH: {
			//case search
			argument = 0;
			for (i = InputCounter; i < InputSize; i++) {
				if (in_cor[i] > d_threshold) {
					if (*d_plateau < min_plateau) {
						(*d_plateau)++;

					} else {
						*d_state = COPY;
						InputCounter = i;
						*OutSize = InputSize - InputCounter;
						*d_copied = 0;
						argument = cargf(in_abs[i]);
						*d_freq_offset = argument / 16;
						*d_plateau = 0;
						break;
					}
				} else {
					*d_plateau = 0;
				}
			}
			InputCounter = i;
			*OutSize = InputSize - i;
			break;
		}

		case COPY: {
			//case copy
			while (InputCounter < InputSize && OutputCounter < *OutSize
					&& *d_copied < MAX_SAMPLES) {
				if (in_cor[InputCounter] > d_threshold) {
					if (*d_plateau < min_plateau) {
						(*d_plateau)++;
						// there's another frame
					} else if (*d_copied > MIN_GAP) {
						ReachMinGap[*IndexReachMinGap] = *d_copied;
						*IndexReachMinGap++;
						*d_copied = 0;
						*d_plateau = 0;
						argument = carg(in_abs[InputCounter]);
						*d_freq_offset = argument / 16;
						break;
					}
					} else {
						*d_plateau = 0;
					}


				ExpPower = 0 + (-1 * (*d_freq_offset) * (*d_copied)) * I;
				out[OutputCounter] = in[InputCounter] * cexpf(ExpPower);

				  if(fwrite(out+OutputCounter, sizeof(float complex), 1, fp) != 1)
				  {
					printf("File write error.");
				  }

				OutputCounter++;
				InputCounter++;
				(*d_copied)++;
			}
			if (*d_copied == MAX_SAMPLES) {
				*d_state = SEARCH;
			}
		}
		}

	}
}

void functionsBeforeSyncShort(float complex* RxInput, short int RxInputSize,
		float complex* Delay16Output, float complex delay_before_sync_short[],
		float complex* MovAvgCompOutput, float* DivideOutput,
		float* sum_MovingAverage, float complex* sum_MovingAverage_Complex,
		float Ref_For_MovingAvergae[],
		float complex Ref_For_MovingAvergae_Complex[]) {
	//declarations
	float* ComptoMagSqOutput = (float *) calloc(RxInputSize, sizeof(float));

	//first path from input
	//complex to magnitude square
	complex2magSquare(RxInput, ComptoMagSqOutput, RxInputSize); // da double

	//moving average for float array
	float* MovAvgFloatOutput = (float *) calloc(RxInputSize, sizeof(float));
	movingAverage(ComptoMagSqOutput, MovAvgFloatOutput, RxInputSize, 64,
			sum_MovingAverage, Ref_For_MovingAvergae); //should be divided by compto mag after moving average complex

	//second path from input
	//delaying the receiver input
	short int Delay16Size = RxInputSize + 16;
	delayy(RxInput, Delay16Output, 16, RxInputSize, delay_before_sync_short);//delay 16 first input to sync short
	memcpy(delay_before_sync_short, RxInput + RxInputSize - 16,
			16 * sizeof(complex float));

	float complex* CompConjugateMultiplyOutput = (float complex*) calloc(
			RxInputSize + 16, sizeof(float complex));//complex conjugate output array
	short int i = 0;
	for (i = 0; i < Delay16Size; i++) {
		CompConjugateMultiplyOutput[i] = conjf(Delay16Output[i]);//conj of delayed frame by 16
	}

	//multiplying receiver input by complex conj of the delayed16 frame
	for (i = 0; i < RxInputSize; i++) {
		CompConjugateMultiplyOutput[i] = RxInput[i]
				* CompConjugateMultiplyOutput[i];		//multiply output
	}

	//moving average
	movingAverageComplex(CompConjugateMultiplyOutput, MovAvgCompOutput,
			RxInputSize, 48, sum_MovingAverage_Complex,
			Ref_For_MovingAvergae_Complex);	//second input to syncshort

	//complex to magnitude
	float* ComptoMagOutput = (float *) calloc(RxInputSize, sizeof(float));
	complex2mag(MovAvgCompOutput, ComptoMagOutput, RxInputSize);

	//dividing both two paths to create the third path to sync short
	for (i = 0; i < RxInputSize; i++) {
		DivideOutput[i] = ComptoMagOutput[i] / MovAvgFloatOutput[i];//third input to sync short
	}

	free(ComptoMagOutput);
	free(CompConjugateMultiplyOutput);
	free(MovAvgFloatOutput);
	free(ComptoMagSqOutput);
}
