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

#include "system_csk_su_base.h"

CskSu::CskSu(class Options &opts) : decFailCount_(0),
  bitEstErrorCount_(0), completionPercentage(opts.nBer), verbose_(opts.verbose),
  nBer_(opts.nBer), bit_(opts.bit), sprType_(opts.spr), spreading(opts.sprPars),
  nseType_(opts.nse), decType_(opts.dec) {
  setSpreadingMethods(sprType_, spreading);
  (*(spreading.initialize))(spreading.pars);
  sprMean_ = (*(spreading.getMean))(spreading.pars);
  sprVar_ = (*(spreading.getVariance))(spreading.pars);

  setNoiseMethods(nseType_, noise);
}

void CskSu::setSnrPerBit(const double snrpb) {
  snrPerBit_ = snrpb;
}

unsigned long int CskSu::getDecFailCount() const {
  return decFailCount_;
}

double CskSu::convertSnrPerBitToNoiseVariance() {
  noise.var = (spr_.size()*sprVar_)/pow(10, snrPerBit_/10);

  return noise.var;
}

double CskSu::convertNoiseVarianceToSnrPerBit() {
  snrPerBit_ = 10*log10(spr_.size()*sprVar_/noise.var);

  return snrPerBit_;
}
