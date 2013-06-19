/*
CSK, Version 1.0. Simulation of Bit Error Rate in CSK Communication Systems.
Copyright (C) 2013 Theodore Papamarkou

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 3, as published by the Free
Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License, version 3, for more
details.

A copy of the GNU General Public License, version 3, has been distributed along
with this CSK software, see the accompanying file LICENSE_1_0.txt, and can also
be found at http://www.gnu.org/licenses/gpl.txt
*/

#ifndef NUMBER_ALGORITHMS_H_
#define NUMBER_ALGORITHMS_H_

bool isEven(const int);

bool isOdd(const int);

template <typename T>
bool isPositive(const T x) {return x>0;}

template <typename T>
int sign(const T x) {return x>=0 ? 1 : -1;}

#endif /* NUMBER_ALGORITHMS_H_ */
