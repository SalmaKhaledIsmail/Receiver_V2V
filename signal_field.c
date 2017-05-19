/*
 * signal_field.c
 *
 *  Created on: Apr 8, 2017
 *      Author: salma
 */

#include "signal_field.h"

   int interleaver_pattern[48] = {
	 0, 3, 6, 9,12,15,18,21,
	24,27,30,33,36,39,42,45,
	 1, 4, 7,10,13,16,19,22,
	25,28,31,34,37,40,43,46,
	 2, 5, 8,11,14,17,20,23,
	26,29,32,35,38,41,44,47};


void deinterleave(uint8_t *rx_bits , uint8_t * d_deinterleaved)
{
	int i;
	for( i = 0; i < 48; i++) {
		d_deinterleaved[i] = rx_bits[interleaver_pattern[i]];
	}


}

bool parse_signal(uint8_t *decoded_bits,unsigned char* d_frame_symbols,unsigned char* d_frame_encoding) {

	unsigned char r = 0;

	unsigned char d_frame_bytes = 0;


	bool parity = false;

	int i ;
	for( i = 0; i < 17; i++)
	{
		parity ^= decoded_bits[i];

		if((i < 4) && decoded_bits[i])
		{
			r = r | (1 << i);
		}

		if(decoded_bits[i] && (i > 4) && (i < 17))
		{
			d_frame_bytes = d_frame_bytes | (1 << (i-5));
		}
	}

	if(parity != decoded_bits[17])
	{
		printf( "SIGNAL: wrong parity\n");
		return false;
	}

	switch(r)
	{
	case 11:
	{
		*d_frame_encoding = 0;
		// This is the number of OFDM symols in the Data->Service(16 bit)+ PSDU + Tail(6 bits) + Pad bits
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 24);
		printf("Encoding: 3 Mbit/s  \n ");
		break;
	}
	case 15:
	{
		*d_frame_encoding = 1;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 36);
		printf("Encoding: 4.5 Mbit/s   \n");
		break;
	}
	case 10:
	{
		*d_frame_encoding = 2;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 48);
		printf("Encoding: 6 Mbit/s  \n ");
		break;
	}
	case 14:
	{
		*d_frame_encoding = 3;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 72);
		printf("Encoding: 9 Mbit/s   \n");
		break;
	}
	case 9:
	{
		*d_frame_encoding = 4;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 96);
		printf("Encoding: 12 Mbit/s  \n ");
		break;
	}
	case 13:
	{
		*d_frame_encoding = 5;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 144);
		printf("Encoding: 18 Mbit/s  \n ");
		break;
	}
	case 8:
	{
		*d_frame_encoding = 6;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 192);
		printf("Encoding: 24 Mbit/s  \n ");
		break;
	}
	case 12:
	{
		*d_frame_encoding = 7;
		*d_frame_symbols = (int) ceil((16 + 8 * d_frame_bytes + 6) / (double) 216);
		printf("Encoding: 27 Mbit/s   \n");
		break;
	}
	default:
	{
		printf("unknown encoding\n");
		return false;
	}
	}
	return true;
}

bool decode_signal_field(uint8_t *rx_bits,unsigned char* d_frame_symbols,unsigned char* d_frame_encoding )
{
	 ofdm_param *   equalizer_decode_signal_ofdm = (ofdm_param *) malloc(sizeof *  equalizer_decode_signal_ofdm );

	 if (equalizer_decode_signal_ofdm == NULL)
	 {
	 		printf("Not enough memory for equalizer_decode_signal_ofdm  \n");
	 }

	 ofdm_param_intialization(BPSK_1_2, equalizer_decode_signal_ofdm);

	 frame_param* equalizer_decode_signal_frame = (frame_param *) malloc(sizeof *  equalizer_decode_signal_frame );
	 if (equalizer_decode_signal_frame == NULL)
	 {
	 		printf("Not enough memory for equalizer_decode_signal_frame  \n");
	 }

	frame_param_intialization( equalizer_decode_signal_ofdm,equalizer_decode_signal_frame,0);


	uint8_t * d_deinterleaved= (uint8_t*) calloc(48, sizeof(uint8_t));
	if (d_deinterleaved == NULL)
	{
		printf("Not enough memory for d_deinterleaved signal field \n");
	}

    deinterleave(rx_bits,d_deinterleaved);

    uint8_t *decoded_bits = decode(equalizer_decode_signal_ofdm, equalizer_decode_signal_frame, d_deinterleaved);

    bool parity = parse_signal(decoded_bits,d_frame_symbols,d_frame_encoding);

    free(d_deinterleaved);
    free(equalizer_decode_signal_frame);
    free(equalizer_decode_signal_ofdm);
	return parity;
}
