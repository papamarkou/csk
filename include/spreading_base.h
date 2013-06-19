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

#ifndef SPREADING_BASE_H_
#define SPREADING_BASE_H_

#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "io.h"
#include "options_parser.h"
#include "sequence_algorithms.h"
#include "spreading_bernoulli.h"
#include "spreading_circular.h"
#include "spreading_logistic.h"
#include "spreading_negative_bernoulli.h"
#include "spreading_pbcs.h"
#include "spreading_tent.h"
#include "spreading_fs2bl.h"
#include "spreading_valley.h"
#include "tee_devices.h"

typedef void (*initializeSpreading)(std::vector<double> &);

typedef double (*getSpreadingMean)(const std::vector<double> &);

typedef double (*getSpreadingVariance)(const std::vector<double> &);

typedef void (*generateSpreading)(boost::numeric::ublas::vector<double> &,
  const std::vector<double> &);

struct Spreading {
    std::vector<double> pars;

    Spreading(const std::vector<double> &);

    initializeSpreading initialize;
    getSpreadingMean getMean;
    getSpreadingVariance getVariance;
    generateSpreading generate;
};

int simulateSpreading(class Options &);

void setSpreadingMethods(const std::string, struct Spreading &);

#endif /* SPREADING_BASE_H_ */
