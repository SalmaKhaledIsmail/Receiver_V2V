/*
 * Copyright (C) 2016 Bastian Bloessl <bloessl@ccs-labs.org>
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

#include "base.h"

const complex float LONG[64] = { 0 + 0*I,  0+ 0*I,  0+ 0*I,  0+ 0*I,  0+ 0*I,  0+ 0*I,  1+ 0*I,  1+ 0*I, -1+ 0*I, -1+ 0*I,
	 1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I,  1+ 0*I,  1+ 0*I,  1+ 0*I,
	 1+ 0*I, -1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I,
	 1+ 0*I,  1+ 0*I,  0+ 0*I,  1+ 0*I, -1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I,
	-1+ 0*I,  1+ 0*I, -1+ 0*I, -1+ 0*I, -1+ 0*I, -1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I, -1+ 0*I,
	-1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I, -1+ 0*I,  1+ 0*I,  1+ 0*I,  1+ 0*I,  1+ 0*I,  0+ 0*I,
	 0 + 0*I,  0 + 0*I,  0 + 0*I,  0 + 0*I};

const complex float POLARITY[127] = {
	 1, 1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1, 1,
	-1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1,
	 1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,
	-1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,
	-1,-1, 1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1,-1,-1,-1,
	-1, 1,-1,-1, 1,-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,
	-1,-1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1, 1, 1, 1,-1,
	-1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1 };
