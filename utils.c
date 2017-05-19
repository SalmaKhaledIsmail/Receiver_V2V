/*
 * utils.c
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

/*
 * utils.c
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#include "utils.h"

/**----------------------------------------------------------ofdm_param_implementation--------------------------------------------**/

void ofdm_param_intialization(Encoding e, ofdm_param* ofdm) {

	ofdm->encoding = e;

	switch (e) {
	case BPSK_1_2:
		ofdm->n_bpsc = 1;
		ofdm->n_cbps = 48;
		ofdm->n_dbps = 24;
		ofdm->rate_field = 0x0D; // 0b00001101
		break;

	case BPSK_3_4:
		ofdm->n_bpsc = 1;
		ofdm->n_cbps = 48;
		ofdm->n_dbps = 36;
		ofdm->rate_field = 0x0F; // 0b00001111
		break;

	case QPSK_1_2:
		ofdm->n_bpsc = 2;
		ofdm->n_cbps = 96;
		ofdm->n_dbps = 48;
		ofdm->rate_field = 0x05; // 0b00000101
		break;

	case QPSK_3_4:
		ofdm->n_bpsc = 2;
		ofdm->n_cbps = 96;
		ofdm->n_dbps = 72;
		ofdm->rate_field = 0x07; // 0b00000111
		break;

	case QAM16_1_2:
		ofdm->n_bpsc = 4;
		ofdm->n_cbps = 192;
		ofdm->n_dbps = 96;
		ofdm->rate_field = 0x09; // 0b00001001
		break;

	case QAM16_3_4:
		ofdm->n_bpsc = 4;
		ofdm->n_cbps = 192;
		ofdm->n_dbps = 144;
		ofdm->rate_field = 0x0B; // 0b00001011
		break;

	case QAM64_2_3:
		ofdm->n_bpsc = 6;
		ofdm->n_cbps = 288;
		ofdm->n_dbps = 192;
		ofdm->rate_field = 0x01; // 0b00000001
		break;

	case QAM64_3_4:
		ofdm->n_bpsc = 6;
		ofdm->n_cbps = 288;
		ofdm->n_dbps = 216;
		ofdm->rate_field = 0x03; // 0b00000011
		break;
	default:
		printf("incorrect encoding type");
		assert(false);
		break;
	}

}

/** a function to print the values of the ofdm_param */
void print_ofdm_param(const ofdm_param* ofdm) {

	printf("OFDM Parameters:   \n");
	printf("encoding 	   : %i\n", ofdm->encoding);
	printf("rate_field     : %i\n", ofdm->rate_field);
	printf("n_bpsc         : %i\n", ofdm->n_bpsc);
	printf("n_cbps         : %i\n", ofdm->n_cbps);
	printf("n_dbps         : %i\n", ofdm->n_dbps);
}

/**----------------------------------------------------------frame_param_implementation-------------------------------------------**/

void frame_param_intialization(ofdm_param* ofdm, frame_param* frame,
		unsigned char psdu_length) {
	frame->psdu_size = psdu_length;

	// number of symbols (17-11)
	frame->n_sym = (unsigned char) ceil(
			(16 + 8 * (frame->psdu_size) + 6) / (float) ofdm->n_dbps);

	frame->n_data_bits =(short int) (frame->n_sym) * (ofdm->n_dbps);

	// number of padding bits (17-13)
	frame->n_pad =(unsigned char) ((frame->n_data_bits) - (16 + 8 * (frame->psdu_size) + 6));

	frame->n_encoded_bits =(short int) ((frame->n_sym) * (ofdm->n_cbps));
}

/** a function to print the values of the frame_param */
void print_frame_param(const frame_param* frame) {

	printf("FRAME Parameters :	\n");
	printf("psdu_size		 :%i\n", frame->psdu_size);
	printf("n_sym 			 :%i\n", frame->n_sym);
	printf("n_pad 			 :%i\n", frame->n_pad);
	printf("n_encoded_bits   :%i\n", frame->n_encoded_bits);
	printf("n_data_bits 	 :%i\n", frame->n_data_bits);
}

/**-----------------------------------------------------------PSDU_Processing-----------------------------------------------------**/
//1-Generate_bits:
void generate_bits(const unsigned char *psdu, char *data_bits,
		frame_param* frame) {

	//printf(" This is the generate bits" );
	// first 16 bits are zero (SERVICE/DATA field)
	memset(data_bits, 0, 16);

	data_bits += 16;
	int i;
	int b;

	for (i = 0; i < frame->psdu_size; i++) {
		for (b = 0; b < 8; b++) {
			data_bits[i * 8 + b] = !!(psdu[i] & (1 << b));
			// uncomment this if you want to print the output from the generate bits
			//printf("%i",data_bits[i * 8 + b]);
		}

	}

}

//2-Scrambling the data
void scramble(const char *input, char *out, frame_param* frame,
		char initial_state) {

	//printf("This is the scrambler \n");
	int state = initial_state;
	int feedback;
	int i;
	for (i = 0; i < frame->n_data_bits; i++) {

		feedback = (!!(state & 64)) ^ (!!(state & 8));
		out[i] = feedback ^ input[i];
		//printf("%i",out[i]);

		state = ((state << 1) & 0x7e) | feedback;
	}

}
//3-Reseting tail bits

void reset_tail_bits(char *scrambled_data, frame_param* frame) {
	memset(scrambled_data + frame->n_data_bits - frame->n_pad - 6, 0,
			6 * sizeof(char));
}

//4-Convolutional encoding

int ones(int n) {
	int sum = 0;
	int i;
	for (i = 0; i < 8; i++) {
		if (n & (1 << i)) {
			sum++;
		}
	}
	return sum;
}

void convolutional_encoding(const char *input, char *out, frame_param* frame) {

	//printf(" This is the Convolutional encoder");
	int state = 0;
	int i;
	for (i = 0; i < frame->n_data_bits; i++) {
		assert(input[i] == 0 || input[i] == 1);
		state = ((state << 1) & 0x7e) | input[i];
		out[i * 2] = ones(state & 0155) % 2;
		out[i * 2 + 1] = ones(state & 0117) % 2;
		//printf("%i",out[i]);
	}

}

//5- Puncturing the data
void puncturing(const char *input, char *out, frame_param* frame,
		ofdm_param* ofdm) {

	int mod;
	int i;
	for (i = 0; i < frame->n_data_bits * 2; i++) {
		switch (ofdm->encoding) {
		case BPSK_1_2:
		case QPSK_1_2:
		case QAM16_1_2:
			*out = input[i];
			out++;

			break;

		case QAM64_2_3:
			if (i % 4 != 3) {
				*out = input[i];
				out++;
			}
			break;

		case BPSK_3_4:
		case QPSK_3_4:
		case QAM16_3_4:
		case QAM64_3_4:
			mod = i % 6;
			if (!(mod == 3 || mod == 4)) {
				*out = input[i];
				out++;

			}

			break;
		default:
			assert(false);
			break;
		}
	}
}

//6-Interleaving data
void interleave(const unsigned  char *input, unsigned char *out, frame_param* frame,
		ofdm_param* ofdm, Enum_reverse reverse) {

	short int* first = (short int*) malloc(ofdm->n_cbps*sizeof(short int));

	if (first == NULL) {
		printf("Not enough memory for first in the interleaver \n");
	}

	short int* second = (short int*) malloc(ofdm->n_cbps*sizeof(short int));
	if (second == NULL) {
		printf("Not enough memory for second in the interleaver \n");
	}

	short int j;
	unsigned char s = max(ofdm->n_bpsc / 2, 1);
	for (j = 0; j < ofdm->n_cbps; j++) {
		first[j] = s * (j / s) + ((j + (short int) (floor(16.0 * j / ofdm->n_cbps))) % s);
	}
	for (j = 0; j < ofdm->n_cbps; j++) {
		second[j] = 16 * j - (ofdm->n_cbps - 1) * (short int) (floor(16.0 * j / ofdm->n_cbps));
	}

	short int k;
	for (j = 0; j < frame->n_sym; j++) {
		for (k = 0; k < ofdm->n_cbps; k++) {

			if (reverse) {
				out[j * ofdm->n_cbps + second[first[k]]] = input[j * ofdm->n_cbps + k];
			} else {
				out[j * ofdm->n_cbps + k] = input[j * ofdm->n_cbps + second[first[k]]];
			}

		}
	}

	free(first);
	free(second);
}

//7-splitting the symbols according to the modulation type :BPSK,QPSK,QAM16,...
void split_symbols(const char *input, char *out, frame_param* frame,
		ofdm_param* ofdm) {

	//printf(" This is the split symbols ");
	int symbols = frame->n_sym * 48;

	int i;
	int k;
	for (i = 0; i < symbols; i++) {
		out[i] = 0;
		for (k = 0; k < ofdm->n_bpsc; k++) {
			assert(*input == 1 || *input == 0);
			out[i] |= (*input << k);
			input++;
			//printf("%c",out[i]);

		}
	}
}
