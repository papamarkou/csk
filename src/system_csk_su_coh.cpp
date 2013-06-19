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

#include "system_csk_su_coh.h"

namespace ub = boost::numeric::ublas;

CskSuCoh::CskSuCoh(class Options &opts) : CskSu(opts), berLbType_(opts.berLb) {
  setDecoder();

  setBerLbCalculationMethod();
}

void CskSuCoh::setSpreadingLength(const unsigned int len) {
  spr_.resize(len); nse_.resize(len); rec_.resize(len);
}

void CskSuCoh::setDecoder() {
  if (nseType_=="white") {
    if (decType_=="corr") {
      decode = &CskSuCoh::decodeInWhiteNoiseWithCorr;
    }
  }
}

int CskSuCoh::decodeInWhiteNoiseWithCorr() {
  dec_ = ub::inner_prod(rec_, spr_);

  if (dec_>0) bitEst_ = 1;
  else if (dec_<0) bitEst_ = -1;
  else if (dec_==0) {
    throw MinMaxError(std::string("Error in ") +
      "\"CskSuCoh::decodeInWhiteNoiseWithCorr\":\n  " +
      "The correlation decoder evaluates to zero");
  }

  return bitEst_;
}

int CskSuCoh::simulate() {
  (*(spreading.generate))(spr_, spreading.pars);
  spr_ -= ub::scalar_vector<double>(spr_.size(), sprMean_);

  (*(noise.sample))(nse_, noise.pars);

  rec_ = bit_*spr_+nse_;

  (this->*decode)();

  return bitEst_;
}

long double CskSuCoh::simulateBer() {
  decFailCount_ = bitEstErrorCount_ = 0;
  if (verbose_==2) {
    completionPercentage.zero();

    logTee << " ";
    logTee.flush();
  }

  for (unsigned long int i=0; i<nBer_; ++i) {
    try {
      simulate();
      if (verbose_==2) {
        (completionPercentage.*(completionPercentage.step))();
      }
    }
    catch(MinMaxError &MinMaxError) {
      decFailCount_ += 1;
      if (verbose_==2) {
        (completionPercentage.*(completionPercentage.step))();
      }

      continue;
    }

    if (bitEst_!=bit_) bitEstErrorCount_ += 1;
  }

  if (decFailCount_!=nBer_) {
    ber_ = static_cast<long double>(bitEstErrorCount_)/(nBer_-decFailCount_);
  }
  else throw DivisionByZero(std::string("Error in ") +
    "\"CskSuCoh::simulateBer\":\n  " +
    "All decoding attempts failed");

  return ber_;
}

void CskSuCoh::setBerLbCalculationMethod() {
  if (berLbType_=="jensen") {
    calculateBerLb = &CskSuCoh::calculateBerJensenLb;
  }
}

double CskSuCoh::calculateBerJensenLb() {
  boost::math::normal dist;

  berLb_ = cdf(dist, -sqrt(spr_.size()*sprVar_/noise.var));

  return berLb_;
}
