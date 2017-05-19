/*
 * constellations_impl.h
 *
 *  Created on: Feb 23, 2017
 *      Author: fc
 */

#ifndef CONSTELLATIONS_IMPL_H_
#define CONSTELLATIONS_IMPL_H_

float *constellation_bpsk_impl();
float *constellation_qpsk_impl();
float *constellation_16qam_impl();
float *constellation_64qam_impl();
unsigned int decision_maker_bpsk(complex float *sample);
unsigned int decision_maker_qpsk(complex float *sample);
unsigned int decision_maker_16qam(complex float *sample);
unsigned int decision_maker_64qam(complex float *sample);
complex float map_to_points(unsigned int value, float* d_constellation);

#endif /* CONSTELLATIONS_IMPL_H_ */
