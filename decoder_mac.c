/*
 * decoder_mac.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */
#include "decoder_mac.h"

void decode_work(unsigned char *d_rx_symbols,unsigned char *out_bytes,frame_param* frame, ofdm_param* ofdm)
{
	unsigned char *d_rx_bits=malloc((frame->n_sym)*48*(ofdm->n_bpsc)*sizeof(unsigned char));
	if (d_rx_bits == NULL) {
			printf("Not enough memory for first in the interleaver \n");
			assert(false);
		}
	unsigned char *deinterleaved_bits=malloc((frame->n_sym)*48*(ofdm->n_bpsc)*sizeof(unsigned char));
	if (deinterleaved_bits == NULL) {
			printf("Not enough memory for first in the interleaver \n");
			assert(false);
		}

	short int i;
	unsigned char k;
// for loop to split ofdm symbols in to bits
	for( i = 0; i < frame->n_sym * 48; i++) {
		for( k = 0; k < ofdm->n_bpsc; k++)
		{
			d_rx_bits[i*ofdm->n_bpsc + k] = !!( d_rx_symbols[i] & (1 << k) );
		}
	}

    interleave(d_rx_bits, deinterleaved_bits,frame, ofdm,reverse);
	unsigned char *decoded =decode(ofdm, frame,deinterleaved_bits);
	descramble(decoded,out_bytes,frame);
	free(deinterleaved_bits);
	free(d_rx_bits);

}
// this function take pointer to input data and pointer to output data and frame parameter
void descramble (unsigned char *decoded_bits,unsigned char *out_bytes, frame_param *frame) {

	unsigned char state = 0;
	short int i;
	for( i = 0; i < 7; i++) {
		if(decoded_bits[i]) {
			state |= 1 << (6 - i);
		}
	}
	out_bytes[0] = state;

	unsigned char feedback;
	unsigned char bit;

	for( i = 7; i < frame->psdu_size*8+16; i++) {
		feedback = ((!!(state & 64))) ^ (!!(state & 8));
		bit = feedback ^ (decoded_bits[i] & 0x1);
		out_bytes[i/8] |= bit << (i%8);
		state = ((state << 1) & 0x7e) | feedback;
	}
}





