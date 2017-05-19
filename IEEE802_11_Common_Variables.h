/*
 * IEEE802_11_Common_Variables.h
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#ifndef IEEE802_11_COMMON_VARIABLES_H_
#define IEEE802_11_COMMON_VARIABLES_H_

typedef unsigned char uint8_t;
typedef signed char sint8_t;
typedef unsigned short uint16_t;
typedef signed short sint16_t;
typedef unsigned long uint32_t;
typedef signed long sint32_t;
typedef unsigned long longuint64_t;
typedef signed long longsint64_t;
typedef float float32_t;
typedef double float64_t;
typedef int* ptoi;
typedef unsigned char bool;
#define true              1
#define false             0
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif /* IEEE802_11_COMMON_VARIABLES_H_ */
