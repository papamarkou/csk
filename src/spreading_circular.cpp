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

#include "spreading_circular.h"

double circularMap(const double x, const std::vector<double> &pars) {
  double y;

  if((-1<=x) && (x<-sqrt(pars[0])))
    y = -sqrt((1-pow(x, 2))/(1-pars[0]));
  else if((-sqrt(pars[0])<=x) && (x<sqrt(pars[0])))
    y = sqrt(1-pow(x, 2)/pars[0]);
  else if((sqrt(pars[0])<=x) && (x<=1))
    y = -sqrt((1-pow(x, 2))/(1-pars[0]));
  else
    throw NotElement(std::string("Error in \"circularMap\":\n ") +
      "The argument passed to the map does not belong to the map's domain");

  return y;
}

void initializeCircularSpreading(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(0.5);
  }
  else if (pars.size()==1) {
    if (pars[0]<=-1 || pars[0]>=1)
      throw NotElement(std::string("Error in ") +
        "\"initializeCircularSpreading\":\n  " +
        "The deforming parameter of the circular map must be in (-1, 1)");
  }
  else {
    throw InvalidIndex(std::string("Error in ") +
      "\"initializeCircularSpreading\":\n  " +
      "The circular map has only one parameter");
  }
}

double getCircularSpreadingMean(const std::vector<double> &pars) {
  return (4*pars[0]-2)/3;
}

double getCircularSpreadingVariance(const std::vector<double> &pars) {
  return 0.5-pow(getCircularSpreadingMean(pars), 2);
}

double initiateCircularSpreading(const std::vector<double> &pars) {
  double x;
  double u = boost::random::uniform_01<>()(rng);

  if((0<=u) && (u<1-pars[0]))
    x = -sqrt((u+pars[0]-1)/(pars[0]-1));
  else if((1-pars[0]<=u) && (u<=1))
    x = sqrt((u+pars[0]-1)/pars[0]);
  else
    throw NotElement(std::string("Error in \"initiateCircularSpreading\":\n ") +
      "The random value drawn from U(0,1) is not in the range [0,1)");

  return x;
}

void generateCircularSpreading(boost::numeric::ublas::vector<double> &spr,
  const std::vector<double> &pars) {
  spr(0) = initiateCircularSpreading(pars);
  for (unsigned int i=1; i<spr.size(); ++i) {
    spr(i) = circularMap(spr(i-1), pars);
  }
}
