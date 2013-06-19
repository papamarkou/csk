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

#include "spreading_fs2bl.h"

void initializeFs2blSpreading(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(0); pars.push_back(0.5); pars.push_back(1);
  }
  else if (pars.size()==3) {
    if (!boost::algorithm::is_increasing(pars.begin(), pars.end()))
      throw InvalidOrder(std::string("Error in ") +
        "\"initializeFs2blSpreading\":\n  " +
        "The parameters of the map must be strictly ordered as in\n  " +
        "domain lower bound < non-centrality < domain upper bound.");
  }
  else {
    throw InvalidIndex(std::string("Error in ") +
      "\"initializeFs2blSpreading\":\n  " +
      "Either specify all three parameters of the map or none.");
  }
}

double getFs2blSpreadingMean(const std::vector<double> &pars) {
  return (pars[0]+pars[2])/2;
}

double getFs2blSpreadingVariance(const std::vector<double> &pars) {
  return pow(pars[2]-pars[0], 2)/12;
}
