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

#include "spreading_logistic.h"

double logisticMap(const double x) {
  return 4*x*(1-x);
}

void initializeLogisticSpreading(std::vector<double> &pars) {
  if (!pars.empty()) {
    throw InvalidIndex(std::string("Error in ") +
      "\"initializeLogisticSpreading\":\n  " +
      "The logistic map doesn't have any parameters");
  }
}

double getLogisticSpreadingMean(const std::vector<double> &pars) {
  return 0.5;
}

double getLogisticSpreadingVariance(const std::vector<double> &pars) {
  return 0.125;
}

void generateLogisticSpreading(boost::numeric::ublas::vector<double> &spr,
  const std::vector<double> &pars) {
  spr(0) = beta_distribution(0.5, 0.5);
  for (unsigned int i=1; i<spr.size(); ++i) {
    spr(i) = logisticMap(spr(i-1));
  }
}
