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

#ifndef MINIMIZATION_GOLDEN_SECTION_H_
#define MINIMIZATION_GOLDEN_SECTION_H_

#include <algorithm>

#include "minimization_sv_base.h"

class GoldenSection : public SingleVariableFunctionMinimization {
    const double tolerance_;
    double xOfMin_, min_;

  public:
    GoldenSection(const double, const double, const double = 1.0e-10);

    double getAbscissa() const;

    template <class T>
    double minimize(T &f);
};

template <class T>
double GoldenSection::minimize(T &f) {
  const double ratio=0.61803399, cRatio=1-ratio;
  double x0 = lower_, x1, x2, x3 = upper_, fx1, fx2;

  if (abs(upper_-middle_)>abs(middle_-lower_)) {
    x1 = middle_, x2 = middle_+cRatio*(upper_-middle_);
  }
  else {
    x2 = middle_, x1 = middle_-cRatio*(middle_-lower_);
  }

  fx1=f(x1), fx2=f(x2);

  while (std::abs(x3-x0)>tolerance_*(std::abs(x1)+std::abs(x2))) {
    if (fx2<fx1) {
      shiftLeft3(x0, x1, x2, ratio*x2+cRatio*x3);
      shifLeft2(fx1, fx2, f(x2));
    }
    else {
      shiftLeft3(x3,x2,x1,ratio*x1+cRatio*x0);
      shifLeft2(fx2, fx1, f(x1));
    }
  }

  if (fx1<fx2) {
    xOfMin_ = x1, min_ = fx1;
  }
  else {
    xOfMin_ = x2, min_ = fx2;
  }

  return min_;
}

#endif /* MINIMIZATION_GOLDEN_SECTION_H_ */
