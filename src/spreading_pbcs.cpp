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

#include "spreading_pbcs.h"

namespace rnd = boost::random;

void initializePbcs(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(0);
  }
  else if (pars.size()!=1) {
    throw std::invalid_argument(std::string("Error in ") +
      "\"initializePbcs\":\n  " +
      "PBCS has only one parameter");
  }
}

double getPbcsMean(const std::vector<double> &pars) {
  return pars[0];
}

double getPbcsVariance(const std::vector<double> &pars) {
  return 0.5;
}

double inverseCdfPbcs(const double x, const double par) {
  double y;

  if ((0<=x) && (x<0.5))
    y = -sqrt(1-2*x)+par;
  else if ((0.5<=x) && (x<=1))
    y = sqrt(2*x-1)+par;
  else
    throw NotElement(std::string("Error in \"inverseCdfPbcs\":\n ") +
      "The argument passed to the map does not belong to the map's domain");

  return y;
}

void generatePbcs(boost::numeric::ublas::vector<double> &spr,
  const std::vector<double> &pars) {
  double r;
  int a;

  rnd::uniform_01<> uniformDist;
  rnd::bernoulli_distribution<> bernoulliDist;

  for (unsigned int i=0; i<spr.size(); ++i) {
    if (isEven(static_cast<int>(i))) {
      r = uniformDist(rng);
      spr(i) = inverseCdfPbcs(r, pars[0]);
    }
    else {
      r = bernoulliDist(rng);
      a = r==0 ? -1 : 1;
      spr(i) = pars[0]+a*sqrt(1-pow(spr(i-1)-pars[0], 2));
    }
  }
}
