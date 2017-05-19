/*
 * main.c
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>

#include "SynchronizationFunctions.h"
#include "syncshort.h"
#include "Sync_long.h"
#include "base.h"
#include "constellations_impl.h"
#include "frame_equalizer_impl.h"
#include "fft.h"
#include "utils.h"
#include "decoder_mac.h"
#include "viterbi_decoder.h"
#include "ComplexConverter.h"

#pragma pack(1)

#define N 64
#define OUTPUT_FFT_SIZE 1152
#define INPUT_EQUALIZER_SIZE 576
/* The length of the message received from the Mac layer */
#define psdu_length  100

//Global variables	//Receiver Input
const int fileSize = 10000;
float complex RxInput[10000];
float w_sp[2 * N];

unsigned char d_psdu[100] = { 4, 2, 0, 46, 0, 96, 8, 205, 55, 166, 0, 32, 214,
		1, 60, 241, 0, 96, 8, 173, 59, 175, 0, 0, 74, 111, 121, 44, 32, 98, 114,
		105, 103, 104, 116, 32, 115, 112, 97, 114, 107, 32, 111, 102, 32, 100,
		105, 118, 105, 110, 105, 116, 121, 44, 10, 68, 97, 117, 103, 104, 116,
		101, 114, 32, 111, 102, 32, 69, 108, 121, 115, 105, 117, 109, 44, 10,
		70, 105, 114, 101, 45, 105, 110, 115, 105, 114, 101, 100, 32, 119, 101,
		32, 116, 114, 101, 97, 103, 51, 33, 182 };
#define MovingAverageFloat 64
#define MovingAverageComplex 48
#define DELAY_TIME1 16
int main(void) {
	// while loop of outsize variables
	short int outsize = 0;
	float* ReadfloatValue;
	FILE *fp;
	long cur = 0;
	short int LoopCounter =0;


	//before synch short variables
	short int RxInputSize;
	float complex* Delay16Output;
	float* DivideOutput;
	float complex* MovAvgCompOutput;
	float sum_moving_average = 0;
	float complex sum_moving_average_complex = 0;
	float ref_array_moving_average[MovingAverageFloat] = {0};
	float complex ref_array_moving_average_complex[MovingAverageComplex] = {0};
	float complex delay_before_sync_short[DELAY_TIME1] = {0};

	//Sync short variables
	float complex* SyncShortOut;
	enum_state d_state = SEARCH;
	unsigned char d_plateau = 0;
	int d_copied=0;
	int ReachMinGap [10];
	int IndexReachMinGap=0;
	float d_freq_offset=0;

	// writing to file sync short variables
	FILE *fp1;
	// Writing to a file
	  if((fp1=fopen("syncshortout.txt", "wb"))==NULL)
	  {
		printf("Cannot open file.\n");
	  }

	while (LoopCounter<=100) {
		//Reading from file
		ReadfloatValue = (float*) malloc(fileSize * 2 * sizeof(float));
		/* read the values for the sake of testing */
		if ((fp = fopen("3azza.txt", "rb")) == NULL) {
			printf("Cannot open file.\n");
		}
		fseek(fp, cur, SEEK_SET);
		if (fread(ReadfloatValue, sizeof(float), fileSize * 2, fp)
				!= fileSize * 2) {
			if (feof(fp))
				printf("Premature end of file.");
			else
				printf("File read error.");
		}
		cur = ftell(fp);
		fclose(fp);
		FloatToComplex(ReadfloatValue, fileSize * 2, RxInput);
		free(ReadfloatValue);

		//implementing all blocks before sync short //autocorrelation process
		//variables declarations
		RxInputSize = sizeof(RxInput) / sizeof(RxInput[0]);
		Delay16Output = (float complex*) calloc(RxInputSize + 16,
				sizeof(float complex));	//first input to syncshort (to be correlated)
		if (Delay16Output == NULL) {
			printf("\nmemory full forDelay16Output\n");
		}
		MovAvgCompOutput = (float complex*) calloc(RxInputSize,
				sizeof(float complex));			//In_absolute
		if (MovAvgCompOutput == NULL) {
			printf("\nmemory full MovAvgCompOutput\n");
		}
		DivideOutput = (float *) calloc(RxInputSize, sizeof(float));//correlated values
		if (DivideOutput == NULL) {
			printf("\nmemory full DivideOutput\n");
		}
		functionsBeforeSyncShort(RxInput, RxInputSize, Delay16Output,delay_before_sync_short,
				MovAvgCompOutput, DivideOutput,&sum_moving_average,&sum_moving_average_complex,ref_array_moving_average,ref_array_moving_average_complex);	//autocorrelation process

		//Start of sync short implementation
		//Function description: it searches for the start of receiving frame leaving the noise in the beginning.
		//then it copies from the start of the frame till the end of the frame (it copies all values above threshold and calculates their frequency offset)
		SyncShortOut=(float complex*) calloc(RxInputSize, sizeof(float complex));
		if(SyncShortOut==NULL)
				{
					printf("\nmemory full SyncShortOut\n");
				}
		syncShort(&d_state,Delay16Output, MovAvgCompOutput,DivideOutput, SyncShortOut,RxInputSize,&outsize,&d_plateau,&d_copied,ReachMinGap,&IndexReachMinGap, &d_freq_offset, fp1) ;

		if(LoopCounter<=100)
		{
			free(SyncShortOut);
			free(DivideOutput);
			free(MovAvgCompOutput);
			free(Delay16Output);
		}
		LoopCounter++;

	}
	fclose(fp1);


	return 0;
}
