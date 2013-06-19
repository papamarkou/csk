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

#ifndef SYSTEM_CSK_SU_BASE_H_
#define SYSTEM_CSK_SU_BASE_H_

#include <boost/numeric/ublas/vector.hpp>

#include "completion_percentage.h"
#include "noise_base.h"
#include "spreading_base.h"

class CskSu {
  protected:
    unsigned long int decFailCount_, bitEstErrorCount_;
    CompletionPercentage completionPercentage;

    int verbose_;

    unsigned long int nBer_;

    int bit_, bitEst_;

    std::string sprType_;
    std::vector<double> sprPars_;
    Spreading spreading;
    double sprMean_, sprVar_;
    boost::numeric::ublas::vector<double> spr_;

    std::string nseType_;
    double snrPerBit_;
    boost::numeric::ublas::vector<double> nse_;

    std::string decType_;
    double dec_;

    long double ber_;
  public:
    Noise noise;

    CskSu(class Options &);

    void setSnrPerBit(const double);

    unsigned long int getDecFailCount() const;

    double convertSnrPerBitToNoiseVariance();
    double convertNoiseVarianceToSnrPerBit();
};

#endif /* SYSTEM_CSK_SU_BASE_H_ */
