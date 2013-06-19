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

#ifndef SPREADING_CIRCULAR_H_
#define SPREADING_CIRCULAR_H_

#include <boost/numeric/ublas/vector.hpp>

#include "exceptions.h"
#include "random_number_generation.h"

double circularMap(const double, const std::vector<double> &);

void initializeCircularSpreading(std::vector<double> &);

double getCircularSpreadingMean(const std::vector<double> &);

double getCircularSpreadingVariance(const std::vector<double> &);

double initiateCircularSpreading(std::vector<double> &);

void generateCircularSpreading(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

#endif /* SPREADING_CIRCULAR_H_ */
