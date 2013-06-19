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

#ifndef RANDOM_NUMBER_GENERATION_H_
#define RANDOM_NUMBER_GENERATION_H_

#include <boost/math/distributions/extreme_value.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "boost/random.hpp"

#include "exceptions.h"
#include "number_algorithms.h"

extern boost::random::mt19937 rng;

double beta_distribution(const double, const double);

void initializeCauchy(std::vector<double> &);

void initializeGumbel(std::vector<double> &);

void initializeNormal(std::vector<double> &);

void initializeStudent(std::vector<double> &);

double getGumbelMean(const std::vector<double> &);

double getNormalMean(const std::vector<double> &);

double getStudentMean(const std::vector<double> &);

double getGumbelVariance(const std::vector<double> &);

double getNormalVariance(const std::vector<double> &);

double getStudentVariance(const std::vector<double> &);

void sampleCauchy(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

void sampleGumbel(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

void sampleNormal(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

void sampleStudent(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

#endif /* RANDOM_NUMBER_GENERATION_H_ */
