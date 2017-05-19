/*
 * ComplexConverter.h
 *
 *  Created on: Mar 26, 2017
 *      Author: DotCom
 */

#ifndef BEFORESYNCFUNCTIONS_H_
#define BEFORESYNCFUNCTIONS_H_
#include "complex.h"

//--------------------------------------------FUNCTIONS DECLRATIONS-------------------------------------//
void ToComplex(float *realimagarray, float complex *ComplexArray,int size);
void SeperateComplex(float complex *ComplexArray,float *realimagarray,int size);


#endif /* COMPLEXCONVERTER_H_ */
