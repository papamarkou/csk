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

#include "spreading_base.h"

namespace ub = boost::numeric::ublas;

Spreading::Spreading(const std::vector<double> &sprPars) : pars(sprPars) {}

int simulateSpreading(class Options &opts) {
  try {
    std::string sprFileHeader;

    boost::filesystem::path sprFile;

    Spreading spreading(opts.sprPars);
    setSpreadingMethods(opts.spr, spreading);

    (*(spreading.initialize))(spreading.pars);

    for (unsigned int i=0; i<opts.sprLen.size(); ++i) {
      ub::vector<double> spr(opts.sprLen[i]);
      ub::matrix<double> sprMatrix(opts.sprLen[i], opts.sprNSeq);

      sprFile = opts.outputRoot;

      for (unsigned long int j=0; j<opts.sprNSeq; ++j) {
        sprFileHeader += "Seq" + boost::lexical_cast<std::string>(j+1) +
          ((j!=(opts.sprNSeq-1)) ? opts.outputColDelim : "");

        (*(spreading.generate))(spr, spreading.pars);

        copyVectorToMatrixCol(spr, sprMatrix, j);
      }

      sprFile = sprFile.generic_string()+
        ".len"+boost::lexical_cast<std::string>(opts.sprLen[i])+".spr.txt";
      printfBoostMatrix(sprMatrix, sprFile, opts.outputColDelim, sprFileHeader);

      sprFileHeader.clear();
    }
  }
  catch(LogicError &LogicError) {
    errorTee << LogicError.what() << "\n";
    exitTeeDevices();
    return 1;
  }

  return 0;
}

void setSpreadingMethods(const std::string spr, struct Spreading &spreading) {
  if (spr=="bernoulli") {
    spreading.initialize = &initializeFs2blSpreading;
    spreading.getMean = &getFs2blSpreadingMean;
    spreading.getVariance = &getFs2blSpreadingVariance;
    spreading.generate = &generateBernoulliSpreading;
  }
  else if (spr=="circular") {
    spreading.initialize = &initializeCircularSpreading;
    spreading.getMean = &getCircularSpreadingMean;
    spreading.getVariance = &getCircularSpreadingVariance;
    spreading.generate = &generateCircularSpreading;
  }
  else if (spr=="gumbel") {
    spreading.initialize = &initializeGumbel;
    spreading.getMean = &getGumbelMean;
    spreading.getVariance = &getGumbelVariance;
    spreading.generate = &sampleGumbel;
  }
  else if (spr=="logistic") {
    spreading.initialize = &initializeLogisticSpreading;
    spreading.getMean = &getLogisticSpreadingMean;
    spreading.getVariance = &getLogisticSpreadingVariance;
    spreading.generate = &generateLogisticSpreading;
  }
  else if (spr=="negbernoulli") {
    spreading.initialize = &initializeFs2blSpreading;
    spreading.getMean = &getFs2blSpreadingMean;
    spreading.getVariance = &getFs2blSpreadingVariance;
    spreading.generate = &generateNegativeBernoulliSpreading;
  }
  else if (spr=="normal") {
    spreading.initialize = &initializeNormal;
    spreading.getMean = &getNormalMean;
    spreading.getVariance = &getNormalVariance;
    spreading.generate = &sampleNormal;
  }
  else if (spr=="pbcs") {
    spreading.initialize = &initializePbcs;
    spreading.getMean = &getPbcsMean;
    spreading.getVariance = &getPbcsVariance;
    spreading.generate = &generatePbcs;
  }
  else if (spr=="student") {
    spreading.initialize = &initializeStudent;
    spreading.getMean = &getStudentMean;
    spreading.getVariance = &getStudentVariance;
    spreading.generate = &sampleStudent;
  }
  else if (spr=="tent") {
    spreading.initialize = &initializeFs2blSpreading;
    spreading.getMean = &getFs2blSpreadingMean;
    spreading.getVariance = &getFs2blSpreadingVariance;
    spreading.generate = &generateTentSpreading;
  }
  else if (spr=="valley") {
    spreading.initialize = &initializeFs2blSpreading;
    spreading.getMean = &getFs2blSpreadingMean;
    spreading.getVariance = &getFs2blSpreadingVariance;
    spreading.generate = &generateValleySpreading;
  }
}
