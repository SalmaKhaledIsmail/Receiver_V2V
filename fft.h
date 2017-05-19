/*
 * fft.h
 *
 *  Created on: Mar 17, 2017
 *      Author: root
 */

#ifndef FFT_H_
#define FFT_H_

#include "IEEE802_11_Common_Variables.h"
extern void gen_twiddle_fft_sp2 (float *w, unsigned char n);
extern void fft(float* input, float* output, int N, float size, unsigned char shift, float* window);
extern void seperateRealImg2(float* input, float* real, float*img, int N);


#endif /* FFT_H_ */
