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

#include "system_csk_su_ncoh.h"

namespace ub = boost::numeric::ublas;

namespace {
  CskSuNcoh *thisPointer;
}

CskSuNcoh::CskSuNcoh(class Options &opts) : CskSu(opts) {
  if (opts.dec=="mcml") {
    nMcml_ = opts.nMcml;
    mcmlDefaultBrkt_ = opts.mcmlDefaultBrkt;
    mcmlTol_ = opts.mcmlTol;
    fWithBitPlusOne_.resize(opts.nMcml);
    fWithBitMinusOne_.resize(opts.nMcml);
  }

  setDecoder();

  thisPointer = this;
  std::copy(opts.mcmlBrktPars.begin(), opts.mcmlBrktPars.end(),
    std::back_inserter(mcmlBrktPars_));
}

void CskSuNcoh::setSpreadingLength(const unsigned int len) {
  spr_.resize(len); nse_.resize(len); rec1_.resize(len); rec2_.resize(len);
}

void CskSuNcoh::setDecoder() {
  if (nseType_=="white") {
    if (decType_=="corr") {
      decode = &CskSuNcoh::decodeInWhiteNoiseWithCorr;
    }
    else if (decType_=="mcml") {
      decode = &CskSuNcoh::decodeInWhiteNoiseWithMcml;
    }
  }
}

int CskSuNcoh::decodeInWhiteNoiseWithCorr() {
  dec_ = ub::inner_prod(rec1_, rec2_);

  if (dec_>0) bitEst_ = 1;
  else if (dec_<0) bitEst_ = -1;
  else if (dec_==0) {
    throw MinMaxError(std::string("Error in ") +
      "\"CskSuNcoh::decodeInWhiteNoiseWithCorr\":\n  " +
      "The correlation decoder evaluates to zero");
  }

  return bitEst_;
}

double CskSuNcoh::mcmlAddendWithBitPlusOne(double var) {
  double y = 0;

  for (unsigned long int i=0; i<nMcml_; ++i) {
    y += exp(-fWithBitPlusOne_(i)/(2*var));
  }
  y /= 2*nMcml_*pow(var, static_cast<double>(spr_.size()));

  return -y;
}

double CskSuNcoh::WrapperToMcmlAddendWithBitPlusOne(double var) {
  return thisPointer->mcmlAddendWithBitPlusOne(var);
}

double CskSuNcoh::mcmlAddendWithBitMinusOne(double var) {
  double y = 0;

  for (unsigned long int i=0; i<nMcml_; ++i) {
    y += exp(-fWithBitMinusOne_(i)/(2*var));
  }
  y /= 2*nMcml_*pow(var, static_cast<double>(spr_.size()));

  return -y;
}

double CskSuNcoh::WrapperToMcmlAddendWithBitMinusOne(double var) {
  return thisPointer->mcmlAddendWithBitMinusOne(var);
}

int CskSuNcoh::decodeInWhiteNoiseWithMcml() {
  ub::vector<double> diff1, diff2, diff3;
  double prod, e1, e2;

  if (mcmlDefaultBrkt_==true) {
    double brkt[2] = {0.5*noise.var, 1.5*noise.var};

    mcmlBrktPars_.assign(&brkt[0], &brkt[0]+2);
  }

  GoldenSection goldenSection(mcmlBrktPars_[0], mcmlBrktPars_[1], mcmlTol_);

  for (unsigned long int i=0; i<nMcml_; ++i) {
    (*(spreading.generate))(spr_, spreading.pars);
    spr_ -= ub::scalar_vector<double>(spr_.size(), sprMean_);

    diff1 = rec1_-spr_; diff2 = rec1_+spr_; diff3 = rec2_-spr_;
    prod = ub::inner_prod(diff3, diff3);
    fWithBitPlusOne_(i) = ub::inner_prod(diff1, diff1)+prod;
    fWithBitMinusOne_(i) = ub::inner_prod(diff2, diff2)+prod;
  }

  goldenSection.bracket(WrapperToMcmlAddendWithBitPlusOne);
  e1 = -goldenSection.minimize(WrapperToMcmlAddendWithBitPlusOne);

  goldenSection.bracket(WrapperToMcmlAddendWithBitMinusOne);
  e2 = -goldenSection.minimize(WrapperToMcmlAddendWithBitMinusOne);

  dec_ = e1-e2;

  if (dec_>0) bitEst_ = 1;
  else if (dec_<0) bitEst_ = -1;
  else if (dec_==0) {
    throw MinMaxError(std::string("Error in ") +
      "\"CskSuNcoh::decodeInWhiteNoiseWithMcml\":\n  " +
      "The MCML decoder evaluates to zero");
  }

  return bitEst_;
}

int CskSuNcoh::simulate() {
  (*(spreading.generate))(spr_, spreading.pars);
  spr_ -= ub::scalar_vector<double>(spr_.size(), sprMean_);

  (*(noise.sample))(nse_, noise.pars);
  rec1_ = bit_*spr_+nse_;

  (*(noise.sample))(nse_, noise.pars);
  rec2_ = spr_+nse_;

  (this->*decode)();

  return bitEst_;
}

long double CskSuNcoh::simulateBer() {
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
    "\"CskSuNcoh::simulateBer\":\n  " +
    "All decoding attempts failed");

  return ber_;
}
