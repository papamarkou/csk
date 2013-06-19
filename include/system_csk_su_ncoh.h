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

#ifndef SYSTEM_CSK_SU_NCOH_H_
#define SYSTEM_CSK_SU_NCOH_H_

#include <boost/numeric/ublas/vector.hpp>

#include "exceptions.h"
#include "minimization_golden_section.h"
#include "system_csk_su_base.h"

class CskSuNcoh : public CskSu {
    unsigned long int nMcml_;

    boost::numeric::ublas::vector<double> rec1_, rec2_;

    bool mcmlDefaultBrkt_;
    std::vector<double> mcmlBrktPars_;
    double mcmlTol_;
    boost::numeric::ublas::vector<double> fWithBitPlusOne_;
    boost::numeric::ublas::vector<double> fWithBitMinusOne_;

    void setDecoder();
    int decodeInWhiteNoiseWithCorr();
    double mcmlAddendWithBitPlusOne(double);
    static double WrapperToMcmlAddendWithBitPlusOne(double);
    double mcmlAddendWithBitMinusOne(double);
    static double WrapperToMcmlAddendWithBitMinusOne(double);
    int decodeInWhiteNoiseWithMcml();
  public:
    CskSuNcoh(class Options &);

    void setSpreadingLength(const unsigned int);

    int (CskSuNcoh::*decode)();

    int simulate();
    long double simulateBer();
};

#endif /* SYSTEM_CSK_SU_NCOH_H_ */
