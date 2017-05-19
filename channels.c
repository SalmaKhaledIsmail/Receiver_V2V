/*
 * channel.c
 *
 *  Created on: Apr 20, 2017
 *      Author: root
 */

#include "channels.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base.h"
#include "constellations_impl.h"


void equalize_ls(complex float *in, unsigned char n, complex float *symbols,unsigned char *bits, unsigned char mod, float* d_snr,
		complex float d_H[64]) {

	if (n == 0)
	{
		memcpy(d_H, in, 64 * sizeof(complex float));

	} else if (n == 1)
	{
		double signal = 0;
		double noise = 0;
		int i;
		for (i = 0; i < 64; i++)
		{
			noise += pow(abs(d_H[i] - in[i]), 2);
			signal += pow(abs(d_H[i] + in[i]), 2);
		}

		*d_snr = 10 * log10(signal / noise / 2);
		for (i = 0; i < 64; i++) {
			d_H[i] += in[i];
			d_H[i] /= LONG[i] * 2;
		}

	} else
	{
		int c = 0;
		int i;
		for (i = 0; i < 64; i++)
		{
			if (!((i == 11) || (i == 25) || (i == 32) || (i == 39) || (i == 53)|| (i < 6) || (i > 58)))
			{
				symbols[c] = in[i] / d_H[i];
				switch (mod)
				{
				case BPSK_12:
				{
					bits[c] = decision_maker_bpsk(&symbols[c]);
					c++;
					break;
				}
				case BPSK_34:
				{
					bits[c] = decision_maker_bpsk(&symbols[c]);
					c++;
					break;
				}
				case QPSK_12:
				{
					bits[c] = decision_maker_qpsk(&symbols[c]);
					c++;
					break;
				}
				case QPSK_34:
				{
					bits[c] = decision_maker_qpsk(&symbols[c]);
					c++;
					break;
				}
				case QAM16_12:
				{
					bits[c] = decision_maker_16qam(&symbols[c]);
					c++;
					break;
				}
				case QAM16_34:
				{
					bits[c] = decision_maker_16qam(&symbols[c]);
					c++;
					break;
				}
				case QAM64_12:
				{
					bits[c] = decision_maker_64qam(&symbols[c]);
					c++;
					break;
				}
				case QAM64_34:
				{
					bits[c] = decision_maker_64qam(&symbols[c]);
					c++;
					break;
				}
				}

			}
		}
	}
}

void equalize_lms(complex float *in, unsigned char n, complex float *symbols, unsigned char *bits,
	                  unsigned char mod,float* d_snr,complex float d_H[64])
	{
		const double alpha = 0.5;
	    if (n == 0)
	    {
	        memcpy(d_H, in, 64 * sizeof(complex float));

	    }
	    else if (n == 1)
	    {
	        double signal = 0;
	        double noise = 0;
	        int i;
	        for (i = 0; i < 64; i++)
	        {
	            noise += pow(abs(d_H[i] - in[i]), 2);
	            signal += pow(abs(d_H[i] + in[i]), 2);
	        }

	        *d_snr = 10 * log10(signal / noise / 2);
	        complex float temp = 2 + 0*I;

	        for (i = 0; i < 64; i++)
	        {
	            d_H[i] += in[i];
	            d_H[i] /= LONG[i] * temp;
	        }

	    }
	    else
	    {
	        int c = 0;
	        int i;
	        for (i = 0; i < 64; i++)
	        {
	            if (!((i == 11) || (i == 25) || (i == 32) || (i == 39) || (i == 53) || (i < 6) || (i > 58)))         //a5er 3 conditions msh fahmahom
	            {
	                symbols[c] = in[i] / d_H[i];
	                float* d_constellation;
	                complex float point = 0;
	                switch(mod)
	                {
	                case(BPSK_12):
	                		bits[c] = decision_maker_bpsk(&symbols[c]);
	                		d_constellation = constellation_bpsk_impl();
	                		point = map_to_points(bits[c],d_constellation);
	                		break;
	                case(BPSK_34):
	                		bits[c] = decision_maker_bpsk(&symbols[c]);
	                		d_constellation = constellation_bpsk_impl();
	                		point = map_to_points(bits[c],d_constellation);
	                		break;

	                case(QPSK_12):
							bits[c] = decision_maker_qpsk(&symbols[c]);
            				d_constellation = constellation_qpsk_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                case(QPSK_34):
							bits[c] = decision_maker_qpsk(&symbols[c]);
            				d_constellation = constellation_qpsk_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                case(QAM16_12):
							bits[c] = decision_maker_16qam(&symbols[c]);
            				d_constellation = constellation_16qam_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                case(QAM16_34):
							bits[c] = decision_maker_16qam(&symbols[c]);
            				d_constellation = constellation_16qam_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                case(QAM64_12):
							bits[c] = decision_maker_64qam(&symbols[c]);
            				d_constellation = constellation_64qam_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                case(QAM64_34):
							bits[c] = decision_maker_64qam(&symbols[c]);
            				d_constellation = constellation_64qam_impl();
            				point = map_to_points(bits[c],d_constellation);
            				break;
	                }
	                free(d_constellation);
	                complex float alpha1 = (1-alpha) + 0*I;
	                complex float alpha2 = alpha + 0*I;
	                d_H[i] = alpha1 * d_H[i] + alpha2 * in[i] / point;
	                c++;
	            }
	        }
	    }
	}

void equalize_sta(complex float *in, unsigned char n, complex float *symbols,
		unsigned char *bits, unsigned char mod, float* d_snr,
		complex float d_H[64]) {
	const double alpha = 0.5;
	const int beta = 2;
	if (n == 0) {
		memcpy(d_H, in, 64 * sizeof(complex float));

	} else if (n == 1) {
		double signal = 0;
		double noise = 0;
		int i;
		for (i = 0; i < 64; i++) {
			noise += pow(abs(d_H[i] - in[i]), 2);
			signal += pow(abs(d_H[i] + in[i]), 2);
		}

		*d_snr = 10 * log10(signal / noise / 2);
		complex float temp = 2 + 0 * I;
		for (i = 0; i < 64; i++) {
			d_H[i] += in[i];
			d_H[i] /= LONG[i] * temp;
		}

	} else {

		complex float H_update[64];
		complex float H[64];

		complex float p = POLARITY[(n - 2) % 127];

		H[11] = in[11] * p;
		H[25] = in[25] * p;
		H[39] = in[39] * p;
		H[53] = in[53] * -p;

		int c = 0;
		int i;
		for (i = 0; i < 64; i++) {
			if (!(i == 11) || (i == 25) || (i == 32) || (i == 39) || (i == 53)
					|| (i < 6) || (i > 58)) {
				symbols[c] = in[i] / d_H[i];
				float* d_constellation;
				complex float point = 0;
				switch (mod) {
				case (BPSK_12):
					bits[c] = decision_maker_bpsk(&symbols[c]);
					d_constellation = constellation_bpsk_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (BPSK_34):
					bits[c] = decision_maker_bpsk(&symbols[c]);
					d_constellation = constellation_bpsk_impl();
					point = map_to_points(bits[c], d_constellation);
					break;

				case (QPSK_12):
					bits[c] = decision_maker_qpsk(&symbols[c]);
					d_constellation = constellation_qpsk_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (QPSK_34):
					bits[c] = decision_maker_qpsk(&symbols[c]);
					d_constellation = constellation_qpsk_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (QAM16_12):
					bits[c] = decision_maker_16qam(&symbols[c]);
					d_constellation = constellation_16qam_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (QAM16_34):
					bits[c] = decision_maker_16qam(&symbols[c]);
					d_constellation = constellation_16qam_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (QAM64_12):
					bits[c] = decision_maker_64qam(&symbols[c]);
					d_constellation = constellation_64qam_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				case (QAM64_34):
					bits[c] = decision_maker_64qam(&symbols[c]);
					d_constellation = constellation_64qam_impl();
					point = map_to_points(bits[c], d_constellation);
					break;
				}
				H[i] = in[i] / point;
				c++;
			}
		}
		for (i = 0; i < 64; i++) {
			int n = 0;
			complex float s = 0;
			int k;
			for (k = i - beta; k <= i + beta; k++) {
				if (!((k == 32) || (k < 6) || (k > 58))) {
					n++;
					s += H[k];
				}
				complex float N = n;
				H_update[i] = s / N;
			}
			complex float alpha1 = (1 - alpha) + 0 * I;
			complex float alpha2 = alpha + 0 * I;
			for (i = 0; i < 64; i++) {
				d_H[i] = alpha1 * d_H[i] + alpha2 * H_update[i];
			}
		}
	}
}

void equalize_comb(complex float *in, unsigned char n, complex float *symbols,
		unsigned char *bits, unsigned char mod, float* d_snr,
		complex float d_H[64]) {
	const double alpha = 0.2;
	*d_snr = 42;
	complex float pilot[4];

	if (n < 2)
	{
		pilot[0] = in[11];
		pilot[1] = -in[25];
		pilot[2] = in[39];
		pilot[3] = in[53];
	}
	else
	{
		complex float p = POLARITY[(n - 2) % 127];
		pilot[0] = in[11] * p;
		pilot[1] = in[25] * p;
		pilot[2] = in[39] * p;
		pilot[3] = in[53] * -p;
	}
	complex float FOUR = 4 + 0 * I;
	complex float avg = (pilot[0] + pilot[1] + pilot[2] + pilot[3]) / FOUR;
	int i;
	for (i = 0; i < 64; i++) {
		complex float H;
		if (i <= 11)
		{
			complex float number1 = ((11-i)/11) + 0*I;
			complex float number2 = (i/11) + 0*I;
			H = number1 * avg
					+ number2 * pilot[0];
		}
		else if (i <= 25)
		{
			complex float number1 = ((25-i)/14) + 0*I;
			complex float number2 = ((i - 11)/14) + 0*I;
			H = number1 * pilot[0]
					+ number2 * pilot[1];
		}
		else if (i <= 39)
		{
			complex float number1 = ((39-i)/14) + 0*I;
			complex float number2 = ((i - 25)/14) + 0*I;
			H = number1 * pilot[1]
					+ number2 * pilot[2];
		}
		else if (i <= 53)
		{
			complex float number1 = ((53-i)/14) + 0*I;
			complex float number2 = ((i - 39)/14) + 0*I;
			H = number1 * pilot[2]
					+ number2 * pilot[3];
		}
		else
		{
			complex float number1 = ((64-i)/11) + 0*I;
			complex float number2 = ((i - 53)/11) + 0*I;
			H = number1 * pilot[3]
					+ number2 * avg;
		}

		if (n == 0)
		{
			d_H[i] = H;
		}
		else
		{
			complex float alpha1 = (1-alpha) + 0*I;
			complex float alpha2 = alpha + 0*I;
			d_H[i] = alpha1 * d_H[i]
					+ alpha2 * H;
		}
	}

	int c = 0;
	for (i = 0; i < 64; i++) {
		if (!(i == 11) || (i == 25) || (i == 32) || (i == 39) || (i == 53)
				|| (i < 6) || (i > 58))
		{
			symbols[c] = in[i] / d_H[i];
            switch(mod)
            {
            case(BPSK_12):
            		bits[c] = decision_maker_bpsk(&symbols[c]);
            case(QPSK_12):
					bits[c] = decision_maker_qpsk(&symbols[c]);
            case(QAM16_12):
					bits[c] = decision_maker_16qam(&symbols[c]);
            case(QAM64_12):
					bits[c] = decision_maker_64qam(&symbols[c]);
            case(BPSK_34):
            		bits[c] = decision_maker_bpsk(&symbols[c]);
            case(QPSK_34):
					bits[c] = decision_maker_qpsk(&symbols[c]);
            case(QAM16_34):
					bits[c] = decision_maker_16qam(&symbols[c]);
            case(QAM64_34):
					bits[c] = decision_maker_64qam(&symbols[c]);
            }
			c++;
		}
	}
}


