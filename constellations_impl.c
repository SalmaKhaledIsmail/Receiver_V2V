#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
//#include "chunks_to_symbols_impl.h"
#include "constellations_impl.h"

//--------------------------------------------FUNCTIONS IMPLEMENTATIONS---------------------------------//
float *constellation_bpsk_impl() {
	float *d_constellation = calloc(2 * 2, sizeof(float)); //as each complex number will take 2 successive bytes
	d_constellation[0] = -1; //first real
	d_constellation[1] = 0; //first imag
	d_constellation[2] = 1; //second imag
	d_constellation[3] = 0; //second imag

	return d_constellation;
}

unsigned int decision_maker_bpsk(complex float *sample) {
	return (creal(*sample) > 0);
}

float *constellation_qpsk_impl() {
	const float level = sqrt((float) (0.5));
	float *d_constellation = calloc(4 * 2, sizeof(float));
	// Gray-coded
	d_constellation[0] = -level;
	d_constellation[1] = -level;
	d_constellation[2] = level;
	d_constellation[3] = -level;
	d_constellation[4] = -level;
	d_constellation[5] = level;
	d_constellation[6] = level;
	d_constellation[7] = level;

	return d_constellation;
}

unsigned int decision_maker_qpsk(complex float *sample) {
	return 2*(cimag(*sample)>0) + (creal(*sample)>0);
}

float *constellation_16qam_impl() {
	const float level = sqrt((float) (0.1));
	float *d_constellation = calloc(16 * 2, sizeof(float));

	d_constellation[0] = -3 * level;
	d_constellation[1] = -3 * level;
	d_constellation[2] = 3 * level;
	d_constellation[3] = -3 * level;
	d_constellation[4] = -1 * level;
	d_constellation[5] = -3 * level;
	d_constellation[6] = 1 * level;
	d_constellation[7] = -3 * level;
	d_constellation[8] = -3 * level;
	d_constellation[9] = 3 * level;
	d_constellation[10] = 3 * level;
	d_constellation[11] = 3 * level;
	d_constellation[12] = -1 * level;
	d_constellation[13] = 3 * level;
	d_constellation[14] = 1 * level;
	d_constellation[15] = 3 * level;
	d_constellation[16] = -3 * level;
	d_constellation[17] = -1*level;
	d_constellation[18] = 3 * level;
	d_constellation[19] = -1 * level;
	d_constellation[20] = -1 * level;
	d_constellation[21] = -1 * level;
	d_constellation[22] = 1 * level;
	d_constellation[23] = -1 * level;
	d_constellation[24] = -3 * level;
	d_constellation[25] = 1 * level;
	d_constellation[26] = 3 * level;
	d_constellation[27] = 1 * level;
	d_constellation[28] = -1 * level;
	d_constellation[29] = 1 * level;
	d_constellation[30] = 1 * level;
	d_constellation[31] = 1 * level;
	return d_constellation;
}


unsigned int decision_maker_16qam(complex float *sample)
{
	unsigned int ret = 0;
	const float level = sqrt(0.1);
	float re = crealf(*sample);
	float im = cimagf(*sample);

	ret |= re > 0;
	float a = cabsf(re);
	ret |= (cabsf(re) < (2*level)) << 1;
	ret |= (im > 0) << 2;
	ret |= (cabsf(im) < (2*level)) << 3;

	return ret;
}

float *constellation_64qam_impl() {
	const float level = sqrt((float) (1 / 42.0));
	float *d_constellation = calloc(16 * 2, sizeof(float));

	d_constellation[0] = -7 * level;
	d_constellation[1] = -7 * level;
	d_constellation[2] = 7 * level;
	d_constellation[3] = -7 * level;
	d_constellation[4] = -1 * level;
	d_constellation[5] = -7 * level;
	d_constellation[6] = 1 * level;
	d_constellation[7] = -7 * level;
	d_constellation[8] = -5 * level;
	d_constellation[9] = -7 * level;
	d_constellation[10] = 5 * level;
	d_constellation[11] = -7 * level;
	d_constellation[12] = -3 * level;
	d_constellation[13] = -7 * level;
	d_constellation[14] = 3 * level;
	d_constellation[15] = -7 * level;
	d_constellation[16] = -7 * level;
	d_constellation[17] = 7 * level;
	d_constellation[18] = 7 * level;
	d_constellation[19] = 7 * level;
	d_constellation[20] = -1 * level;
	d_constellation[21] = 7 * level;
	d_constellation[22] = 1 * level;
	d_constellation[23] = 7 * level;
	d_constellation[24] = -5 * level;
	d_constellation[25] = 7 * level;
	d_constellation[26] = 5 * level;
	d_constellation[27] = 7 * level;
	d_constellation[28] = -3 * level;
	d_constellation[29] = 7 * level;
	d_constellation[30] = 3 * level;
	d_constellation[31] = 7 * level;
	d_constellation[32] = -7 * level;
	d_constellation[33] = -1 * level;
	d_constellation[34] = 7 * level;
	d_constellation[35] = -1 * level;
	d_constellation[36] = -1 * level;
	d_constellation[37] = -1 * level;
	d_constellation[38] = 1 * level;
	d_constellation[39] = -1 * level;
	d_constellation[40] = -5 * level;
	d_constellation[41] = -1 * level;
	d_constellation[42] = 5 * level;
	d_constellation[43] = -1 * level;
	d_constellation[44] = -3 * level;
	d_constellation[45] = -1 * level;
	d_constellation[46] = 3 * level;
	d_constellation[47] = -1 * level;
	d_constellation[48] = -7 * level;
	d_constellation[49] = 1 * level;
	d_constellation[50] = 7 * level;
	d_constellation[51] = 1 * level;
	d_constellation[52] = -1 * level;
	d_constellation[53] = 1 * level;
	d_constellation[54] = 1 * level;
	d_constellation[55] = 1 * level;
	d_constellation[56] = -5 * level;
	d_constellation[57] = 1 * level;
	d_constellation[58] = 5 * level;
	d_constellation[59] = 1 * level;
	d_constellation[60] = -3 * level;
	d_constellation[61] = 1 * level;
	d_constellation[62] = 3 * level;
	d_constellation[63] = 1 * level;
	d_constellation[64] = -7 * level;
	d_constellation[65] = -5 * level;
	d_constellation[66] = 7 * level;
	d_constellation[67] = -5 * level;
	d_constellation[68] = -1 * level;
	d_constellation[69] = -5 * level;
	d_constellation[70] = 1 * level;
	d_constellation[71] = -5 * level;
	d_constellation[72] = -5 * level;
	d_constellation[73] = -5 * level;
	d_constellation[74] = 5 * level;
	d_constellation[75] = -5 * level;
	d_constellation[76] = -3 * level;
	d_constellation[77] = -5 * level;
	d_constellation[78] = 3 * level;
	d_constellation[79] = -5 * level;
	d_constellation[80] = -7 * level;
	d_constellation[81] = 5 * level;
	d_constellation[82] = 7 * level;
	d_constellation[83] = 5 * level;
	d_constellation[84] = -1 * level;
	d_constellation[85] = 5 * level;
	d_constellation[86] = 1 * level;
	d_constellation[87] = 5 * level;
	d_constellation[88] = -5 * level;
	d_constellation[89] = 5 * level;
	d_constellation[90] = 5 * level;
	d_constellation[91] = 5 * level;
	d_constellation[92] = -3 * level;
	d_constellation[93] = 5 * level;
	d_constellation[94] = 3 * level;
	d_constellation[95] = 5 * level;
	d_constellation[96] = -7 * level;
	d_constellation[97] = -3 * level;
	d_constellation[98] = 7 * level;
	d_constellation[99] = -3 * level;
	d_constellation[100] = -1 * level;
	d_constellation[101] = -3 * level;
	d_constellation[102] = 1 * level;
	d_constellation[103] = -3 * level;
	d_constellation[104] = -5 * level;
	d_constellation[105] = -3 * level;
	d_constellation[106] = 5 * level;
	d_constellation[107] = -3 * level;
	d_constellation[108] = -3 * level;
	d_constellation[109] = -3 * level;
	d_constellation[110] = 3 * level;
	d_constellation[111] = -3 * level;
	d_constellation[112] = -7 * level;
	d_constellation[113] = 3 * level;
	d_constellation[114] = 7 * level;
	d_constellation[115] = 3 * level;
	d_constellation[116] = -1*level;
	d_constellation[117] = 3 * level;
	d_constellation[118] = 1 * level;
	d_constellation[119] = 3 * level;
	d_constellation[120] = -5 * level;
	d_constellation[121] = 3 * level;
	d_constellation[122] = 5 * level;
	d_constellation[123] = 3 * level;
	d_constellation[124] = -3 * level;
	d_constellation[125] = 3 * level;
	d_constellation[126] = 3 * level;
	d_constellation[127] = 3 * level;

	return d_constellation;
}


unsigned int decision_maker_64qam(complex float *sample)
{
	unsigned int ret = 0;
	const float level = sqrt(1/42.0);
	float re = creal(*sample);
	float im = cimag(*sample);

	ret |= re > 0;
	ret |= (abs(re) < (4*level)) << 1;
	ret |= (abs(re) < (6*level) && abs(re) > (2*level)) << 2;
	ret |= (im > 0) << 3;
	ret |= (abs(im) < (4*level)) << 4;
	ret |= (abs(im) < (6*level) && abs(im) > (2*level)) << 5;

	return ret;
}
complex float map_to_points(unsigned int value, float* d_constellation)
{
	complex float points;
    points = d_constellation[2*value] + d_constellation[2*value+1]*I;
    return points;
}

