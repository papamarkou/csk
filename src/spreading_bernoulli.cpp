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

#include "spreading_bernoulli.h"

double bernoulliMap(const double x, const std::vector<double> &pars) {
  double y;

  // pars[0] = lower bound, pars[1] = upper bound, pars[2] = non-centrality
  if ((pars[0]<=x) && (x<pars[1]))
    y = ((pars[2]-pars[0])*x+(pars[1]-pars[2])*pars[0])/(pars[1]-pars[0]);
  else if ((pars[1]<=x) && (x<=pars[2]))
    y = ((pars[2]-pars[0])*x+(pars[0]-pars[1])*pars[2])/(pars[2]-pars[1]);
  else
    throw NotElement(std::string("Error in \"bernoulliMap\":\n ") +
      "The argument passed to the map does not belong to the map's domain");

  return y;
}

void generateBernoulliSpreading(boost::numeric::ublas::vector<double> &spr,
  const std::vector<double> &pars) {
  spr(0) =
    boost::random::uniform_real_distribution<>(pars[0], pars[2])(rng);
  for (unsigned int i=1; i<spr.size(); ++i) {
    spr(i) = bernoulliMap(spr(i-1), pars);
  }
}
