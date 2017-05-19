/*
 * frame_equalizer_impl.h
 *
 *  Created on: Mar 25, 2017
 *      Author: root
 */

#ifndef FRAME_EQUALIZER_IMPL_H_
#define FRAME_EQUALIZER_IMPL_H_
#include <complex.h>
enum equalizer {LS, LMS, STA, COMB};

#define	 d_freq  5.98e9
#define offset  0.000523329
#define d_bw 20e6

void frame_equalizer_work(complex float* in, unsigned char* out, unsigned char size);

#endif /* FRAME_EQUALIZER_IMPL_H_ */
