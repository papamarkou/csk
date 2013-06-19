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

#ifndef COMPLETION_PERCENTAGE_H_
#define COMPLETION_PERCENTAGE_H_

#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

#include "tee_devices.h"

class CompletionPercentage {
    std::string precedingOutput_, currentOutput_;
    unsigned long int totalIterations_, currentIteration_;

    ldiv_t totalDiv_, currentDiv_;
    double currentPercentage_;

    void setStep();
    void stepWithNGreaterThan100();
    void stepWithNMultipleOf100();
    void stepWithNLessThan100();
  public:
    CompletionPercentage(unsigned long int);

    void zero();
    void (CompletionPercentage::*step)();
};

#endif /* COMPLETION_PERCENTAGE_H_ */
