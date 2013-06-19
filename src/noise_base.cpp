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

#include "noise_base.h"

void setNoiseMethods(const std::string ns, struct Noise &noise) {
  if (ns=="white") {
    noise.initialize = &initializeWhiteNoise;
    noise.sample = &sampleNormal;
  }
}

void initializeWhiteNoise(const double noiseVar,
  const std::vector<double> &noiseOptPars, std::vector<double> &noisePars) {
  if (!isPositive(noiseVar))
    throw InvalidSign(std::string("Error in \"initializeWhiteNoise\":\n  ") +
      "The variance of white noise must be positive");

  noisePars.push_back(0); noisePars.push_back(sqrt(noiseVar));
}
