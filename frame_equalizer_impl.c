/*
 * frame_equalizer_impl.c
 *
 *  Created on: Mar 25, 2017
 *      Author: root
 */

#include "frame_equalizer_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include "channels.h"
#include "base.h"
#include "utils.h"
#include "signal_field.h"
#include <math.h>
#include <complex.h>
#define M_PI		3.14159265358979323846
#define printfc(c) printf("%f%c%f i",creal(c),(cimag(c)>=0.0f)? '+':'\0',cimag(c))


void frame_equalizer_work(complex float* in, unsigned char* out, unsigned char size)
{
	unsigned char i = 0,d_frame_symbols = 0,d_current_symbol = 0,o=0,d_equalizer = LS, d_frame_mod = BPSK_12;

	complex float symbols[48];
	complex float current_symbol[64];

	complex float d_prev_pilots[4] = { 0 + 0 * I, 0 + 0 * I, 0 + 0 * I, 0 + 0 * I };

	double d_epsilon0;
	double d_er;
	d_epsilon0 = offset * d_bw / (2 * M_PI * d_freq);
	d_er = 0;

	complex float H[64];
	float snr;

	unsigned char k;
	complex float comp;
	float img;
	complex float  p;
	float beta;
	float er;
	complex float b;

	float alpha = 0.1;

	while (o < size && i<size)
	{
		if (!(d_current_symbol > (d_frame_symbols + 2)))
		{
			memcpy(current_symbol, in + i * 64, 64 * sizeof(complex float));
			// compensate sampling offset
			for (k = 0; k < 64; k++)
			{
				img = (2 * M_PI * d_current_symbol * 80 * (d_epsilon0 + d_er) * (k - 32) / 64);
				comp = 0 + img * I;
				current_symbol[k] *= cexpf(comp);
			}
			p = POLARITY[(d_current_symbol - 2) % 127];
			if (d_current_symbol < 2)
			{
				beta = cargf(current_symbol[11] - current_symbol[25]+ current_symbol[39] + current_symbol[53]); // fil LONG 1, -1, 1, 1

			}
			else
			{
				beta = cargf((current_symbol[11] * p) + (current_symbol[39] * p)+ (current_symbol[25] * p)+ (current_symbol[53] * -p));
			}

			er =cargf((conjf(d_prev_pilots[0]) * current_symbol[11] * p)+ (conjf(d_prev_pilots[1])* current_symbol[25] * p)
							+ (conjf(d_prev_pilots[2])* current_symbol[39] * p)+ (conjf(d_prev_pilots[3])* current_symbol[53] * -p));

			er *= d_bw / (2 * M_PI * d_freq * 80);

			d_prev_pilots[0] = current_symbol[11] * p;
			d_prev_pilots[1] = current_symbol[25] * p;
			d_prev_pilots[2] = current_symbol[39] * p;
			d_prev_pilots[3] = current_symbol[53] * -p;

			// compensate residual frequency offset
			b = 0 - beta * I;
			for (k = 0; k < 64; k++)
			{
				current_symbol[k] *= cexpf(b);
			}

			// update estimate of residual frequency offset
			if (d_current_symbol >= 2)
			{
				d_er = (1 - alpha) * d_er + alpha * er;
			}

			if (d_equalizer == LS)
			{
				equalize_ls(current_symbol, d_current_symbol, symbols,out + o * 48, d_frame_mod, &snr, H);
			}


			// signal field
			if (d_current_symbol == (unsigned char) 2)
			{

				if (decode_signal_field(out + o * 48,&d_frame_symbols,&d_frame_mod))
				{

				}
			}

			if (d_current_symbol > 2)
			{
				o++;
			}

			i++;
			d_current_symbol++;
		}
		else
		{
			i++;
		}
	}
}
