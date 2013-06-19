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

#ifndef SYSTEM_CSK_SU_COH_H_
#define SYSTEM_CSK_SU_COH_H_

#include "exceptions.h"
#include "system_csk_su_base.h"

class CskSuCoh : public CskSu {
    boost::numeric::ublas::vector<double> rec_;

    std::string berLbType_;
    double berLb_;

    void setDecoder();
    int decodeInWhiteNoiseWithCorr();

    void setBerLbCalculationMethod();
    double calculateBerJensenLb();
  public:
    CskSuCoh(class Options &);

    void setSpreadingLength(const unsigned int);

    int (CskSuCoh::*decode)();

    int simulate();
    long double simulateBer();

    double (CskSuCoh::*calculateBerLb)();
};

#endif /* SYSTEM_CSK_SU_COH_H_ */
