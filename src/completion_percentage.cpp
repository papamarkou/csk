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

#include "completion_percentage.h"

CompletionPercentage::CompletionPercentage(unsigned long int totalIterations) :
  totalIterations_(totalIterations), currentIteration_(0) {
  totalDiv_ = ldiv(totalIterations_, 100);

  setStep();
}

void CompletionPercentage::setStep() {
  if (totalIterations_>=100) {
    if (totalDiv_.rem!=0) {
      step = &CompletionPercentage::stepWithNGreaterThan100;
    }
    else if (totalDiv_.rem==0) {
      step = &CompletionPercentage::stepWithNMultipleOf100;
    }
  }
  else if (totalIterations_<100) {
    step = &CompletionPercentage::stepWithNLessThan100;
  }
}

void CompletionPercentage::zero() {
  precedingOutput_.clear();
  currentIteration_ = 0;
}

void CompletionPercentage::stepWithNGreaterThan100() {
  currentIteration_ += 1;
  currentDiv_ = ldiv(currentIteration_, totalDiv_.quot);

  if ((currentDiv_.quot>totalDiv_.rem && currentDiv_.rem==0) ||
    (0<currentDiv_.quot && currentDiv_.quot<=totalDiv_.rem &&
      currentDiv_.rem==1)) {
    currentOutput_ = boost::lexical_cast<std::string>(currentDiv_.quot) + "%";
    logTee << std::string(precedingOutput_.length(),'\b') << currentOutput_;
    logTee.flush();
    precedingOutput_ = currentOutput_;
  }
}

void CompletionPercentage::stepWithNMultipleOf100() {
  currentIteration_ += 1;
  currentDiv_ = ldiv(currentIteration_, totalDiv_.quot);

  if (currentDiv_.rem==0) {
    currentOutput_ = boost::lexical_cast<std::string>(currentDiv_.quot) + "%";
    logTee << std::string(precedingOutput_.length(),'\b') << currentOutput_;
    logTee.flush();
    precedingOutput_ = currentOutput_;
  }
}

void CompletionPercentage::stepWithNLessThan100() {
  currentIteration_ += 1;
  currentPercentage_ = boost::math::round(static_cast<double>
    (currentIteration_)/totalIterations_*100);

  currentOutput_ = boost::lexical_cast<std::string>(currentPercentage_) + "%";
  logTee << std::string(precedingOutput_.length(),'\b') << currentOutput_;
  logTee.flush();
  precedingOutput_ = currentOutput_;
}
