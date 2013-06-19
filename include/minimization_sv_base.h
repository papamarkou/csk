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

#ifndef MINIMIZATION_SV_BASE_H_
#define MINIMIZATION_SV_BASE_H_

#include <cmath>

#include "exceptions.h"
#include "number_algorithms.h"

class SingleVariableFunctionMinimization {
  protected:
    double lower_, middle_, upper_, fOfLower_, fOmMiddle_, fOfUpper_;

  public:
    SingleVariableFunctionMinimization(const double, const double);

    inline void shifLeft2(double &x1, double &x2, const double x3) {
      x1 = x2, x2 = x3;
    }

    inline void shiftLeft3(double &x1, double &x2, double &x3,
      const double x4) {
      x1 = x2, x2 = x3, x3 = x4;
    }

    template <class T>
    void bracket(T &f);
};

template <class T>
void SingleVariableFunctionMinimization::bracket(T &f) {
    const double goldenRatio = 1.618033988, maxStep = 100, epsilon = 1e-20;
    double x1, x2, x3, x3Lim, fx3;

    fOfLower_ = f(lower_), fOmMiddle_ = f(middle_);

    if (fOmMiddle_>fOfLower_) {
      std::swap(lower_, middle_); std::swap(fOmMiddle_, fOfLower_);
    }

    upper_ = middle_+goldenRatio*(middle_-lower_), fOfUpper_ = f(upper_);

    while (fOmMiddle_>fOfUpper_) {
      x1 = (middle_-lower_)*(fOmMiddle_-fOfUpper_);
      x2 = (middle_-upper_)*(fOmMiddle_-fOfLower_);
      x3 = middle_-((middle_-upper_)*x2-(middle_-lower_)*x1)/
        (2.0*sign(x2-x1)*std::max(std::abs(x2-x1), epsilon));
      x3Lim = middle_+maxStep*(upper_-middle_);

      if ((middle_-x3)*(x3-upper_)>0) {
        fx3 = f(x3);

        if (fx3<fOfUpper_) {
          shifLeft2(lower_, middle_, x3);
          shifLeft2(fOfLower_, fOmMiddle_, fx3);

          return;
        }
        else if (fx3>fOmMiddle_) {
          upper_ = x3, fOfUpper_ = fx3;

          return;
        }

        x3 = upper_+goldenRatio*(upper_-middle_), fx3 = f(x3);
      }
      else if ((upper_-x3)*(x3-x3Lim)>0) {
        fx3 = f(x3);

        if (fx3<fOfUpper_) {
          shiftLeft3(middle_, upper_, x3, x3+goldenRatio*(x3-upper_));
          shiftLeft3(fOmMiddle_, fOfUpper_, fx3, f(x3));
        }
      }
      else if ((x3-x3Lim)*(x3Lim-upper_)>=0) {
        x3 = x3Lim, fx3 = f(x3);
      }
      else {
        x3 = upper_+goldenRatio*(upper_-middle_), fx3 = f(x3);
      }

      shiftLeft3(lower_, middle_, upper_, x3);
      shiftLeft3(fOfLower_, fOmMiddle_, fOfUpper_, fx3);
    }

    if (!(lower_<middle_ && middle_<upper_ &&
      fOfLower_>fOmMiddle_ && fOmMiddle_<fOfUpper_)) {
      throw MinMaxError(std::string("Error in ") +
        "\"SingleVariableFunctionMinimization::bracket\":\n  " +
        "Not possible to bracket the given function in the given interval");
    }
}

#endif /* MINIMIZATION_SV_BASE_H_ */
