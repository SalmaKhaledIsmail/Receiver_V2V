/*
 * base.h
 *
 *  Created on: Mar 19, 2017
 *      Author: root
 */

#ifndef BASE_H_
#define BASE_H_

#include <complex.h>

enum mod { BPSK_12 = 0
	,BPSK_34 = 1, QPSK_12 = 2,QPSK_34 = 3, QAM16_12 = 4,QAM16_34 = 5, QAM64_12 = 6,QAM64_34 = 7 };
extern const complex float  POLARITY[127];
extern const complex float LONG[64];

#endif /* BASE_H_ */
/*
 * Copyright (C) 2015 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
