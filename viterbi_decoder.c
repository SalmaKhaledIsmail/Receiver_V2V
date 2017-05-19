/*
 * viterbi_decoder.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */
#include "viterbi_decoder.h"

// Position in circular buffer where the current decoded byte is stored
int d_store_pos;
// Metrics for each state
unsigned char d_mmresult[64] __attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//

unsigned char d_ppresult[TRACEBACK_MAX][64] __attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//

union branchtab27 {
	unsigned char c[32];
} d_branchtab27_sse2[2];

unsigned char d_metric0[64]__attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//
unsigned char d_metric1[64]__attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//
unsigned char d_path0[64]__attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//
unsigned char d_path1[64]__attribute__((aligned(16))); //*3ayzen n3raf dah leah aligned 16*//

int d_ntraceback;
int d_k;
ofdm_param *d_ofdm;
frame_param *d_frame;
const unsigned char *d_depuncture_pattern;

unsigned char d_depunctured[MAX_ENCODED_BITS]; //*3ayzen na5aly dah optimized shwaya we mayknsh static array na5aleh pointer to point le makan n7gzh leha fe recet function *//
unsigned char d_decoded[MAX_ENCODED_BITS * 3 / 4]; //*3ayzen na5aly dah optimized shwaya we mayknsh static array na5aleh pointer to point le makan n7gzh leha fe recet function  function *//

static const unsigned char PARTAB[256];
static const unsigned char PUNCTURE_1_2[2];
static const unsigned char PUNCTURE_2_3[4];
static const unsigned char PUNCTURE_3_4[6];



// return address of static array which contain decoded data
unsigned char * decode(ofdm_param *ofdm, frame_param *frame, unsigned char *in){

	d_ofdm = ofdm;
	d_frame = frame;

	reset();
	unsigned char *depunctured = depuncture(in);

	int in_count = 0;
	int out_count = 0;
	int n_decoded = 0;
	int i;

	while(n_decoded < d_frame->n_data_bits) {

		if ((in_count % 4) == 0) { //0 or 3
			viterbi_butterfly2_sse2(&depunctured[in_count & 0xfffffffc], d_metric0, d_metric1, d_path0, d_path1);

			if ((in_count > 0) && (in_count % 16) == 8) { // 8 or 11
				unsigned char c;

				viterbi_get_output_sse2(d_metric0, d_path0, d_ntraceback, &c);

				if (out_count >= d_ntraceback) {
					for ( i= 0; i < 8; i++) {
						d_decoded[(out_count - d_ntraceback) * 8 + i] = (c >> (7 - i)) & 0x1;
						n_decoded++;

					}
				}
				out_count++;
			}
		}
		in_count++;
	}

	return d_decoded;
}

void viterbi_butterfly2_sse2(unsigned char *symbols, unsigned char mm0[],
		unsigned char mm1[], unsigned char pp0[], unsigned char pp1[]) {
	int i, j, k;

	unsigned char *metric0, *metric1;
	unsigned char *path0, *path1;

	metric0 = mm0;
	path0 = pp0;
	metric1 = mm1;
	path1 = pp1;

	// Operate on 4 symbols (2 bits) at a time
	unsigned char *m0=calloc(16,sizeof(unsigned char));
	if (m0 == NULL) {
		printf("Not enough memory for m0 \n");
	}
	unsigned char *m1=calloc(16,sizeof(unsigned char));
	if (m1 == NULL) {
			printf("Not enough memory for m1 \n");
		}
	unsigned char *m2=calloc(16,sizeof(unsigned char));
	if (m2 == NULL) {
			printf("Not enough memory for m2 \n");
		}
	unsigned char *m3=calloc(16,sizeof(unsigned char));
	if (m3 == NULL) {
			printf("Not enough memory for m3 \n");
		}
	unsigned char *decision0=calloc(16,sizeof(unsigned char));
	if (decision0 == NULL) {
			printf("Not enough memory for decision0 \n");
		}
	unsigned char *decision1=calloc(16,sizeof(unsigned char));
	if (decision1 == NULL) {
				printf("Not enough memory for decision1 \n");
			}
	unsigned char *survivor0=calloc(16,sizeof(unsigned char));
	if (survivor0 == NULL) {
				printf("Not enough memory for survivor0 \n");
			}
	unsigned char *survivor1=calloc(16,sizeof(unsigned char));
	if (survivor1 == NULL) {
				printf("Not enough memory for survivor1 \n");
			}


	unsigned char *metsv=calloc(16,sizeof(unsigned char));
	if (metsv == NULL) {
				printf("Not enough memory for metsv \n");
			}
	unsigned char *metsvm=calloc(16,sizeof(unsigned char));
	if (metsvm == NULL) {
				printf("Not enough memory for metsvm \n");
			}
	unsigned char *shift0=calloc(16,sizeof(unsigned char));
	if (shift0 == NULL) {
				printf("Not enough memory for shift0 \n");
			}
	unsigned char *shift1=calloc(16,sizeof(unsigned char));
	if (shift1 == NULL) {
					printf("Not enough memory for shift1 \n");
				}
	unsigned char *tmp0=calloc(16,sizeof(unsigned char));
	if (tmp0 == NULL) {
					printf("Not enough memory for tmp0 \n");
				}
	unsigned char *tmp1=calloc(16,sizeof(unsigned char));
	if (tmp1 == NULL) {
					printf("Not enough memory for tmp1 \n");
				}
	unsigned char *sym0v=calloc(16,sizeof(unsigned char));
	if (sym0v == NULL) {
					printf("Not enough memory for sym0v \n");
				}
	unsigned char *sym1v=calloc(16,sizeof(unsigned char));
	if (sym1v == NULL) {
					printf("Not enough memory for sym1v \n");
				}


	unsigned short simd_epi16;

	for (j = 0; j < 16; j++) {
		sym0v[j] = symbols[0];
		sym1v[j] = symbols[1];
	}

	for (i = 0; i < 2; i++) {
		if (symbols[0] == 2) {
			for (j = 0; j < 16; j++) {
				metsvm[j] = d_branchtab27_sse2[1].c[(i * 16) + j] ^ sym1v[j];
				metsv[j] = 1 - metsvm[j];
			}
		} else if (symbols[1] == 2) {
			for (j = 0; j < 16; j++) {
				metsvm[j] = d_branchtab27_sse2[0].c[(i * 16) + j] ^ sym0v[j];
				metsv[j] = 1 - metsvm[j];
			}
		} else {
			for (j = 0; j < 16; j++) {
				metsvm[j] = (d_branchtab27_sse2[0].c[(i * 16) + j] ^ sym0v[j])
						+ (d_branchtab27_sse2[1].c[(i * 16) + j] ^ sym1v[j]);
				metsv[j] = 2 - metsvm[j];
			}
		}
		// assume 2nh bay3ad kam bits motshabha
		for (j = 0; j < 16; j++) {
			m0[j] = metric0[(i * 16) + j] + metsv[j]; //light blue
			m1[j] = metric0[((i + 2) * 16) + j] + metsvm[j]; //dark blue
			m2[j] = metric0[(i * 16) + j] + metsvm[j]; // dark blue
			m3[j] = metric0[((i + 2) * 16) + j] + metsv[j]; //light blue
		}

		for (j = 0; j < 16; j++) {
			decision0[j] = ((m0[j] - m1[j]) > 0) ? 0xff : 0x0;
			decision1[j] = ((m2[j] - m3[j]) > 0) ? 0xff : 0x0;
			survivor0[j] = (decision0[j] & m0[j]) | ((~decision0[j]) & m1[j]);
			survivor1[j] = (decision1[j] & m2[j]) | ((~decision1[j]) & m3[j]);
		}

		for (j = 0; j < 16; j += 2) {
			simd_epi16 = path0[(i * 16) + j];
			simd_epi16 |= path0[(i * 16) + (j + 1)] << 8;
			simd_epi16 <<= 1;
			shift0[j] = simd_epi16;
			shift0[j + 1] = simd_epi16 >> 8;

			simd_epi16 = path0[((i + 2) * 16) + j];
			simd_epi16 |= path0[((i + 2) * 16) + (j + 1)] << 8;
			simd_epi16 <<= 1;
			shift1[j] = simd_epi16;
			shift1[j + 1] = simd_epi16 >> 8;
		}
		for (j = 0; j < 16; j++) {
			shift1[j] = shift1[j] + 1;
		}

		for (j = 0, k = 0; j < 16; j += 2, k++) {
			metric1[(2 * i * 16) + j] = survivor0[k];
			metric1[(2 * i * 16) + (j + 1)] = survivor1[k];
		}
		for (j = 0; j < 16; j++) {
			tmp0[j] = (decision0[j] & shift0[j])
					| ((~decision0[j]) & shift1[j]);
		}

		for (j = 0, k = 8; j < 16; j += 2, k++) {
			metric1[((2 * i + 1) * 16) + j] = survivor0[k];
			metric1[((2 * i + 1) * 16) + (j + 1)] = survivor1[k];
		}
		for (j = 0; j < 16; j++) {
			tmp1[j] = (decision1[j] & shift0[j])
					| ((~decision1[j]) & shift1[j]);
		}

		for (j = 0, k = 0; j < 16; j += 2, k++) {
			path1[(2 * i * 16) + j] = tmp0[k];
			path1[(2 * i * 16) + (j + 1)] = tmp1[k];
		}
		for (j = 0, k = 8; j < 16; j += 2, k++) {
			path1[((2 * i + 1) * 16) + j] = tmp0[k];
			path1[((2 * i + 1) * 16) + (j + 1)] = tmp1[k];
		}
	}
    metric0 = mm1;
    path0 = pp1;
    metric1 = mm0;
    path1 = pp0;

    for (j = 0; j < 16; j++) {
      sym0v[j] = symbols[2];
      sym1v[j] = symbols[3];
    }

    for (i = 0; i < 2; i++) {
      if (symbols[2] == 2) {
        for (j = 0; j < 16; j++) {
          metsvm[j] =d_branchtab27_sse2[1].c[(i*16) + j] ^ sym1v[j];
          metsv[j] = 1 - metsvm[j];
        }
      }
      else if (symbols[3] == 2) {
        for (j = 0; j < 16; j++) {
          metsvm[j] =d_branchtab27_sse2[0].c[(i*16) + j] ^ sym0v[j];
          metsv[j] = 1 - metsvm[j];
        }
      }
      else {
        for (j = 0; j < 16; j++) {
          metsvm[j] = (d_branchtab27_sse2[0].c[(i*16) + j] ^ sym0v[j]) + (d_branchtab27_sse2[1].c[(i*16) + j] ^ sym1v[j]);
          metsv[j] = 2 - metsvm[j];
        }
      }

      for (j = 0; j < 16; j++) {
        m0[j] = metric0[(i*16) + j] + metsv[j];
        m1[j] = metric0[((i+2)*16) + j] + metsvm[j];
        m2[j] = metric0[(i*16) + j] + metsvm[j];
        m3[j] = metric0[((i+2)*16) + j] + metsv[j];
      }

      for (j = 0; j < 16; j++) {
        decision0[j] = ((m0[j] - m1[j]) > 0) ? 0xff : 0x0;
        decision1[j] = ((m2[j] - m3[j]) > 0) ? 0xff : 0x0;
        survivor0[j] = (decision0[j] & m0[j]) | ((~decision0[j]) & m1[j]);
        survivor1[j] = (decision1[j] & m2[j]) | ((~decision1[j]) & m3[j]);
      }

      for (j = 0; j < 16; j += 2) {
        simd_epi16 = path0[(i*16) + j];
        simd_epi16 |= path0[(i*16) + (j+1)] << 8;
        simd_epi16 <<= 1;
        shift0[j] = simd_epi16;
        shift0[j+1] = simd_epi16 >> 8;

        simd_epi16 = path0[((i+2)*16) + j];
        simd_epi16 |= path0[((i+2)*16) + (j+1)] << 8;
        simd_epi16 <<= 1;
        shift1[j] = simd_epi16;
        shift1[j+1] = simd_epi16 >> 8;
      }
      for (j = 0; j < 16; j++) {
        shift1[j] = shift1[j] + 1;
      }

      for (j = 0, k = 0; j < 16; j += 2, k++) {
        metric1[(2*i*16) + j] = survivor0[k];
        metric1[(2*i*16) + (j+1)] = survivor1[k];
      }
      for (j = 0; j < 16; j++) {
        tmp0[j] = (decision0[j] & shift0[j]) | ((~decision0[j]) & shift1[j]);
      }

      for (j = 0, k = 8; j < 16; j += 2, k++) {
        metric1[((2*i+1)*16) + j] = survivor0[k];
        metric1[((2*i+1)*16) + (j+1)] = survivor1[k];
      }
      for (j = 0; j < 16; j++) {
        tmp1[j] = (decision1[j] & shift0[j]) | ((~decision1[j]) & shift1[j]);
      }

      for (j = 0, k = 0; j < 16; j += 2, k++) {
        path1[(2*i*16) + j] = tmp0[k];
        path1[(2*i*16) + (j+1)] = tmp1[k];
      }
      for (j = 0, k = 8; j < 16; j += 2, k++) {
        path1[((2*i+1)*16) + j] = tmp0[k];
        path1[((2*i+1)*16) + (j+1)] = tmp1[k];
      }
    }
	free(sym1v);
	free(sym0v);
	free(tmp1);
	free(tmp0);
	free(shift1);
	free(shift0);
	free(metsvm);
	free(metsv);
	free(survivor1);
	free(survivor0);
	free(decision1);
	free(decision0);
	free(m3);
	free(m2);
	free(m1);
	free(m0);
}

unsigned char viterbi_get_output_sse2(unsigned char *mm0, unsigned char *pp0,
		int ntraceback, unsigned char *outbuf) {
	//  Find current best path
	int i;
	int bestmetric, minmetric;
	int beststate = 0;
	int pos = 0;
	int j;

	// Implement a circular buffer with the last ntraceback paths
	d_store_pos = (d_store_pos + 1) % ntraceback;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 16; j++) {
			d_mmresult[(i * 16) + j] = mm0[(i * 16) + j];
			d_ppresult[d_store_pos][(i * 16) + j] = pp0[(i * 16) + j];
		}
	}

	// Find out the best final state
	bestmetric = d_mmresult[beststate];
	minmetric = d_mmresult[beststate];

	for (i = 1; i < 64; i++) {
		if (d_mmresult[i] > bestmetric) {
			bestmetric = d_mmresult[i];
			beststate = i;
		}
		if (d_mmresult[i] < minmetric) {
			minmetric = d_mmresult[i];
		}
	}
	// Trace back
	for (i = 0, pos = d_store_pos; i < (ntraceback - 1); i++) {
		// Obtain the state from the output bits
		// by clocking in the output bits in reverse order.
		// The state has only 6 bits
		beststate = d_ppresult[pos][beststate] >> 2;
		pos = (pos - 1 + ntraceback) % ntraceback;
	}

	// Store output byte
	*outbuf = d_ppresult[pos][beststate];
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 16; j++) {
			pp0[(i * 16) + j] = 0;
			mm0[(i * 16) + j] = mm0[(i * 16) + j] - minmetric;
		}
	}

	return bestmetric;
}

void reset() {

	viterbi_chunks_init_sse2();

	switch (d_ofdm->encoding) {
	case BPSK_1_2:
	case QPSK_1_2:
	case QAM16_1_2:
		d_ntraceback = 5;
		d_depuncture_pattern = PUNCTURE_1_2;
		d_k = 1;
		break;
	case QAM64_2_3:
		d_ntraceback = 9;
		d_depuncture_pattern = PUNCTURE_2_3; /* used in depuncturing function */
		d_k = 2; /* used in depuncturing function (2x2 puncture matrix) */
		break;
	case BPSK_3_4:
	case QPSK_3_4:
	case QAM16_3_4:
	case QAM64_3_4:
		d_ntraceback = 10;
		d_depuncture_pattern = PUNCTURE_3_4; /* used in depuncturing function */
		d_k = 3; /* used in depuncturing function (3x3 puncture matrix) */
		break;
	}
}

unsigned char* depuncture(unsigned char *in) {

	int count;
	int n_cbps = d_ofdm->n_cbps;
	unsigned char *depunctured;

	if (d_ntraceback == 5) {
		count = d_frame->n_sym * n_cbps;
		depunctured = in;
		/* as no puncturing dne in case of rate 1_2*/

	} else {
		depunctured = d_depunctured;
		count = 0;
		int i;
		int k;
		for (i = 0; i < d_frame->n_sym; i++) {
			for (k = 0; k < n_cbps; k++) {
				while (d_depuncture_pattern[count % (2 * d_k)] == 0) {
					depunctured[count] = 2;
					count++;
				}

				// Insert received bits
				depunctured[count] = in[i * n_cbps + k];
				count++;

				while (d_depuncture_pattern[count % (2 * d_k)] == 0) {
					depunctured[count] = 2;
					count++;
				}
			}
		}
	}

	return depunctured;
}

void viterbi_chunks_init_sse2() {
	int i, j;

	for (i = 0; i < 64; i++) {
		d_metric0[i] = 0;
		d_path0[i] = 0;
	}

	int polys[2] = { 0x6d, 0x4f };
	for (i = 0; i < 32; i++) {
		d_branchtab27_sse2[0].c[i] =
				(polys[0] < 0) ^ PARTAB[(2 * i) & abs(polys[0])] ? 1 : 0;
		d_branchtab27_sse2[1].c[i] =
				(polys[1] < 0) ^ PARTAB[(2 * i) & abs(polys[1])] ? 1 : 0;
	}

	for (i = 0; i < 64; i++) {
		d_mmresult[i] = 0;
		for (j = 0; j < TRACEBACK_MAX; j++) {
			d_ppresult[j][i] = 0;
		}
	}
}
const unsigned char PARTAB[256] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
		1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, };

const unsigned char PUNCTURE_1_2[2] = { 1, 1 };
const unsigned char PUNCTURE_2_3[4] = { 1, 1, 1, 0 };
const unsigned char PUNCTURE_3_4[6] = { 1, 1, 1, 0, 0, 1 };

