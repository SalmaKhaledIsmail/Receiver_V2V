/*
 * channels.h
 *
 *  Created on: Apr 20, 2017
 *      Author: root
 */

#ifndef CHANNELS_H_
#define CHANNELS_H_
#include <complex.h>
void equalize_ls(complex float *in, unsigned char n, complex float *symbols, unsigned char *bits, unsigned char mod,float* d_snr,complex float d_H[64]);
void equalize_lms(complex float *in, unsigned char n, complex float *symbols, unsigned char *bits,
	                  unsigned char mod,float* d_snr,complex float d_H[64]);
void equalize_sta(complex float *in, unsigned char n, complex float *symbols, unsigned char *bits, unsigned char mod,float* d_snr,complex float d_H[64]);
void equalize_comb(complex float *in, unsigned char n, complex float *symbols, unsigned char *bits, unsigned char mod,float* d_snr,complex float d_H[64]);



#endif /* CHANNELS_H_ */
