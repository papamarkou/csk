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

#include "system_base.h"
#include <boost/numeric/conversion/cast.hpp>

namespace fs = boost::filesystem;
namespace ub = boost::numeric::ublas;

int simulateBer(class Options &opts) {
  Timer timer;

  if (opts.sys=="csk-coh-su") {
    try {
      CskSuCoh cskSuCoh(opts);

      unsigned int totalBerIterations;
      unsigned int currentBerIteration = 0;
      double totalBerIterationsPercentage;

      fs::path decFile = opts.outputRoot;
      decFile = decFile.generic_string()+".dec.error.txt";

      fs::path berFile = opts.outputRoot;
      berFile = berFile.generic_string()+".ber.txt";
      std::string berFileHeader = "SNR" + opts.outputColDelim;
      std::vector<unsigned long int> berFileUncheckedCols;
      berFileUncheckedCols.push_back(1);

      unsigned int nseVarLen =
        ((!opts.nseVar.empty()) ? opts.nseVar.size() : opts.snrPerBit.size());
      ub::vector<double> nseVar(nseVarLen), snrPerBit(nseVarLen);
      totalBerIterations = opts.sprLen.size()*nseVarLen;

      ub::vector<unsigned long int> decFailCount(nseVarLen);
      ub::matrix<unsigned long int> decFailCountMatrix(nseVarLen,
        opts.sprLen.size());

      ub::vector<long double> ber(nseVarLen);
      ub::matrix<long double> berMatrix(nseVarLen, opts.sprLen.size()+1);

      cskSuCoh.noise.optPars = opts.nsePars;

      for (unsigned int i=0; i<opts.sprLen.size(); ++i) {
        cskSuCoh.setSpreadingLength(opts.sprLen[i]);

        berFileHeader +=
          "SpreadingLen" + boost::lexical_cast<std::string>(opts.sprLen[i]) +
          ((i!=(opts.sprLen.size()-1)) ? opts.outputColDelim : "");

        for (unsigned int j=0; j<nseVar.size(); ++j) {
          timer.start();
          currentBerIteration += 1;

          if (!opts.nseVar.empty()) {
            nseVar(j) = opts.nseVar[j];
            cskSuCoh.noise.var = nseVar(j);
            snrPerBit(j) = cskSuCoh.convertNoiseVarianceToSnrPerBit();
          }
          else if (!opts.snrPerBit.empty()) {
            snrPerBit(j) = opts.snrPerBit[j];
            cskSuCoh.setSnrPerBit(snrPerBit(j));
            nseVar(j) = cskSuCoh.convertSnrPerBitToNoiseVariance();
          }

          logTee << "Simulating BER point " << currentBerIteration <<
            " of " << totalBerIterations <<
            " (spreading length = " << opts.sprLen[i] <<
            ", SNR = " << snrPerBit(j) << + ")...";
          logTee.flush();

          cskSuCoh.noise.var = nseVar(j);
          cskSuCoh.noise.pars.clear();
          (*(cskSuCoh.noise.initialize))(cskSuCoh.noise.var,
            cskSuCoh.noise.optPars, cskSuCoh.noise.pars);

          try {
            ber(j) = cskSuCoh.simulateBer();

            timer.stop();
            logTee << "\n  BER point " << currentBerIteration <<
              " of " << totalBerIterations <<
              " (spreading length = " << opts.sprLen[i] <<
              ", SNR = " << snrPerBit(j) << + ") simulated in " <<
              displaySecondsAsDays(timer.getEllapsedTime()) << "\n";

            totalBerIterationsPercentage =
              boost::math::round(static_cast<double>
              (currentBerIteration)/totalBerIterations*100);
            logTee << "  " << totalBerIterationsPercentage <<
              "% of BER points processed in " <<
              displaySecondsAsDays(timer.getTotalEllapsedTime()) << "\n\n";

            logTee.flush();
          }
          catch(DivisionByZero &DivisionByZero) {
            ber(j) = -1.;

            logTee << "\n  Simulation of BER point " << currentBerIteration <<
              " of " << totalBerIterations <<
              " (spreading length = " << opts.sprLen[i] <<
              ", SNR = " << snrPerBit(j) << + ") failed.\n";

            totalBerIterationsPercentage =
              boost::math::round(static_cast<double>
              (currentBerIteration)/totalBerIterations*100);
            logTee << "  " << totalBerIterationsPercentage <<
              "% of BER points processed in " <<
              displaySecondsAsDays(timer.getTotalEllapsedTime()) << "\n";

            logTee.flush();
          }

          decFailCount(j) = cskSuCoh.getDecFailCount();
        }

        copyVectorToMatrixCol(decFailCount, decFailCountMatrix, i);
        copyVectorToMatrixCol(ber, berMatrix, i+1);
      }

      copyAnyTypeVectorToMatrixCol(snrPerBit, berMatrix, 0);

      printfBoostMatrix(decFailCountMatrix, decFile, opts.outputColDelim);

      printfBoostMatrixCheckingNa(berMatrix, berFile,
        static_cast<long double>(-1), "NA", opts.outputColDelim, berFileHeader,
        std::vector<unsigned long int>(), berFileUncheckedCols);

      if (!opts.berLb.empty()) {
        if (opts.verbose==2) logTee << "Calculating BER lower bound...";

        fs::path berLbFile = opts.outputRoot;
        berLbFile = berLbFile.generic_string()+".berLb.txt";

        ub::vector<double> berLb(nseVarLen);
        ub::matrix<double> berLbMatrix(nseVarLen, opts.sprLen.size());

        for (unsigned int i=0; i<opts.sprLen.size(); ++i) {
          cskSuCoh.setSpreadingLength(opts.sprLen[i]);

          for (unsigned int j=0; j<nseVar.size(); ++j) {
            cskSuCoh.noise.var = nseVar(j);
            cskSuCoh.noise.pars.clear();
            (*(cskSuCoh.noise.initialize))(cskSuCoh.noise.var,
              cskSuCoh.noise.optPars, cskSuCoh.noise.pars);

            berLb(j) = (cskSuCoh.*cskSuCoh.calculateBerLb)();
          }

          copyVectorToMatrixCol(berLb, berLbMatrix, i);
        }

        printfBoostMatrix(berLbMatrix, berLbFile, opts.outputColDelim);

        if (opts.verbose==2) {
          logTee << " done\n\n";
          logTee.flush();
        }
      }
    }
    catch(LogicError &error) {
      errorTee << error.what() << "\n";
      exitTeeDevices();
      return 1;
    }
  }
  else if (opts.sys=="csk-ncoh-su") {
    try {
      CskSuNcoh cskSuNcoh(opts);

      unsigned int totalBerIterations;
      unsigned int currentBerIteration = 0;
      double totalBerIterationsPercentage;

      fs::path decFile = opts.outputRoot;
      decFile = decFile.generic_string()+".dec.error.txt";

      fs::path berFile = opts.outputRoot;
      berFile = berFile.generic_string()+".ber.txt";
      std::string berFileHeader = "SNR" + opts.outputColDelim;
      std::vector<unsigned long int> berFileUncheckedCols;
      berFileUncheckedCols.push_back(1);

      unsigned int nseVarLen =
        ((!opts.nseVar.empty()) ? opts.nseVar.size() : opts.snrPerBit.size());
      ub::vector<double> nseVar(nseVarLen), snrPerBit(nseVarLen);
      totalBerIterations = opts.sprLen.size()*nseVarLen;

      ub::vector<unsigned long int> decFailCount(nseVarLen);
      ub::matrix<unsigned long int> decFailCountMatrix(nseVarLen,
        opts.sprLen.size());

      ub::vector<long double> ber(nseVarLen);
      ub::matrix<long double> berMatrix(nseVarLen, opts.sprLen.size()+1);

      cskSuNcoh.noise.optPars = opts.nsePars;

      for (unsigned int i=0; i<opts.sprLen.size(); ++i) {
        cskSuNcoh.setSpreadingLength(opts.sprLen[i]);

        berFileHeader +=
          "SpreadingLen" + boost::lexical_cast<std::string>(opts.sprLen[i]) +
          ((i!=(opts.sprLen.size()-1)) ? opts.outputColDelim : "");

        for (unsigned int j=0; j<nseVar.size(); ++j) {
          timer.start();
          currentBerIteration += 1;

          if (!opts.nseVar.empty()) {
            nseVar(j) = opts.nseVar[j];
            cskSuNcoh.noise.var = nseVar(j);
            snrPerBit(j) = cskSuNcoh.convertNoiseVarianceToSnrPerBit();
          }
          else if (!opts.snrPerBit.empty()) {
            snrPerBit(j) = opts.snrPerBit[j];
            cskSuNcoh.setSnrPerBit(snrPerBit(j));
            nseVar(j) = cskSuNcoh.convertSnrPerBitToNoiseVariance();
          }

          logTee << "Simulating BER point " << currentBerIteration <<
            " of " << totalBerIterations <<
            " (spreading length = " << opts.sprLen[i] <<
            ", SNR = " << snrPerBit(j) << + ")...";
          logTee.flush();

          cskSuNcoh.noise.var = nseVar(j);
          cskSuNcoh.noise.pars.clear();
          (*(cskSuNcoh.noise.initialize))(cskSuNcoh.noise.var,
            cskSuNcoh.noise.optPars, cskSuNcoh.noise.pars);

          try {
            ber(j) = cskSuNcoh.simulateBer();

            timer.stop();
            logTee << "\n  BER point " << currentBerIteration <<
              " of " << totalBerIterations <<
              " (spreading length = " << opts.sprLen[i] <<
              ", SNR = " << snrPerBit(j) << + ") simulated in " <<
              displaySecondsAsDays(timer.getEllapsedTime()) << "\n";

            totalBerIterationsPercentage =
              boost::math::round(static_cast<double>
              (currentBerIteration)/totalBerIterations*100);
            logTee << "  " << totalBerIterationsPercentage <<
              "% of BER points processed in " <<
              displaySecondsAsDays(timer.getTotalEllapsedTime()) << "\n\n";

            logTee.flush();
          }
          catch(DivisionByZero &DivisionByZero) {
            ber(j) = -1.;

            logTee << "\n  Simulation of BER point " << currentBerIteration <<
              " of " << totalBerIterations <<
              " (spreading length = " << opts.sprLen[i] <<
              ", SNR = " << snrPerBit(j) << + ") failed.\n";

            totalBerIterationsPercentage =
              boost::math::round(static_cast<double>
              (currentBerIteration)/totalBerIterations*100);
            logTee << "  " << totalBerIterationsPercentage <<
              "% of BER points processed in " <<
              displaySecondsAsDays(timer.getTotalEllapsedTime()) << "\n";

            logTee.flush();
          }

          decFailCount(j) = cskSuNcoh.getDecFailCount();
        }

        copyVectorToMatrixCol(decFailCount, decFailCountMatrix, i);
        copyVectorToMatrixCol(ber, berMatrix, i+1);
      }

      copyAnyTypeVectorToMatrixCol(snrPerBit, berMatrix, 0);

      printfBoostMatrix(decFailCountMatrix, decFile, opts.outputColDelim);

      printfBoostMatrixCheckingNa(berMatrix, berFile,
        static_cast<long double>(-1), "NA", opts.outputColDelim, berFileHeader,
        std::vector<unsigned long int>(), berFileUncheckedCols);
    }
    catch(LogicError &error) {
      errorTee << error.what() << "\n";
      exitTeeDevices();
      return 1;
    }
  }

  return 0;
}
