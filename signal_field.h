/*
 * signal_field.h
 *
 *  Created on: Apr 8, 2017
 *      Author: salma
 */

#ifndef SIGNAL_FIELD_H_
#define SIGNAL_FIELD_H_


#include "viterbi_decoder.h"


void deinterleave(uint8_t *rx_bits,uint8_t *d_deinterleaved);

bool parse_signal(uint8_t *decoded_bits,unsigned char* d_frame_symbols,unsigned char* d_frame_encoding);

bool decode_signal_field(uint8_t *rx_bits,unsigned char* d_frame_symbols,unsigned char* d_frame_encoding) ;



#endif /* SIGNAL_FIELD_H_ */
