/*
 * viterbi_decoder.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */

#ifndef VITERBI_DECODER_H_
#define VITERBI_DECODER_H_

#include "utils.h"

// Maximum number of traceback bytes
#define TRACEBACK_MAX 24

// to reset all variables used in decoding
void reset();
// take pointer to input data and return pointer to output data
unsigned char* depuncture(unsigned char *in);

void viterbi_chunks_init_sse2();

void viterbi_butterfly2_sse2(unsigned char *symbols, unsigned char mm0[],
		unsigned char mm1[], unsigned char pp0[], unsigned char pp1[]);

unsigned char viterbi_get_output_sse2(unsigned char *mm0, unsigned char *pp0,
		int ntraceback, unsigned char *outbuf);

unsigned char * decode(ofdm_param *ofdm, frame_param *frame, unsigned char *in);

#endif /* VITERBI_DECODER_H_ */
