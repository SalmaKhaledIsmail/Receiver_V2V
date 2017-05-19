/*
 * decoder_mac.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */

#ifndef DECODER_MAC_H_
#define DECODER_MAC_H_

#include "viterbi_decoder.h"

void decode_work(unsigned char *d_rx_symbols,unsigned char *out_bytes,frame_param* frame, ofdm_param* ofdm);
void descramble (unsigned char *decoded_bits,unsigned char *out_bytes, frame_param *frame);


#endif /* DECODER_MAC_H_ */
