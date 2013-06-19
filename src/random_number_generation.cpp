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

#include "random_number_generation.h"

namespace math = boost::math;
namespace rnd = boost::random;

rnd::mt19937 rng;

double beta_distribution(const double a, const double b) {
  double x1 = rnd::gamma_distribution<>(a, 1.0)(rng);
  double x2 = rnd::gamma_distribution<>(b, 1.0)(rng);

  return x1/(x1+x2);
}

void initializeCauchy(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(0); pars.push_back(1);
  }
  else if (pars.size()==1) {
    pars.push_back(1);
  }
  else if (pars.size()==2) {
    if (!isPositive(pars[1]))
      throw InvalidSign(std::string("Error in \"initializeCauchy\":\n  ") +
        "The scale parameter of the Cauchy distribution must be positive");
  }
  else {
    throw InvalidIndex(std::string("Error in \"initializeCauchy\":\n  ") +
      "The Cauchy distribution can not take more than two parameters,\n  " +
      "namely its location and scale parameters");
  }
}

void initializeGumbel(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(1); pars.push_back(1);
  }
  else if (pars.size()==1) {
    pars.push_back(1);
  }
  else if (pars.size()==2) {
    if (!isPositive(pars[1]))
      throw InvalidSign(std::string("Error in \"initializeGumbel\":\n  ") +
        "The scale parameter of the Gumbel distribution must be positive");
  }
  else {
    throw InvalidIndex(std::string("Error in \"initializeGumbel\":\n  ") +
      "The Gumbel distribution can not take more than two parameters,\n  " +
      "namely its location and scale parameters");
  }
}

void initializeNormal(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(0); pars.push_back(1);
  }
  else if (pars.size()==1) {
    pars.push_back(1);
  }
  else if (pars.size()==2) {
    if (!isPositive(pars[1]))
      throw InvalidSign(std::string("Error in \"initializeNormal\":\n  ") +
        "The standard deviation of the Normal distribution must be positive");
  }
  else {
    throw InvalidIndex(std::string("Error in \"initializeNormal\":\n  ") +
      "The Normal distribution can not take more than two parameters,\n  " +
      "namely its mean and standard deviation");
  }
}

void initializeStudent(std::vector<double> &pars) {
  if (pars.empty()) {
    pars.push_back(1);
  }
  else if (pars.size()==1) {
    if (!isPositive(pars[0]))
      throw InvalidSign(std::string("Error in \"initializeStudent\":\n  ") +
        "The degrees of freedom of the Student's t distribution " +
        "must be positive");
  }
  else {
    throw InvalidIndex(std::string("Error in \"initializeStudent\":\n  ") +
      "The Student's t distribution can not take more than one parameter,\n  " +
      "namely its degrees of freedom");
  }
}

double getGumbelMean(const std::vector<double> &pars) {
  return mean(math::extreme_value_distribution<>(pars[0], pars[1]));
}

double getNormalMean(const std::vector<double> &pars) {
  return mean(math::normal_distribution<>(pars[0], pars[1]));
}

double getStudentMean(const std::vector<double> &pars) {
  if (pars[0]!=1) {
    return mean(math::students_t_distribution<>(pars[0]));
  }
  else {
    throw NonExistent(std::string("Error in \"getStudentMean\":\n  ") +
      "The mean of the Student's t distribution with one degree of freedom " +
      "is undefined");
  }
}

double getGumbelVariance(const std::vector<double> &pars) {
  return variance(math::extreme_value_distribution<>(pars[0], pars[1]));
}

double getNormalVariance(const std::vector<double> &pars) {
  return variance(math::normal_distribution<>(pars[0], pars[1]));
}

double getStudentVariance(const std::vector<double> &pars) {
  return variance(math::students_t_distribution<>(pars[0]));
}

void sampleCauchy(boost::numeric::ublas::vector<double> &sample,
  const std::vector<double> &pars) {
  rnd::cauchy_distribution<> dist(pars[0], pars[1]);

  for (unsigned long int i=0; i<sample.size(); ++i) sample(i) = dist(rng);
}

void sampleGumbel(boost::numeric::ublas::vector<double> &sample,
  const std::vector<double> &pars) {
  rnd::extreme_value_distribution<> dist(pars[0], pars[1]);

  for (unsigned long int i=0; i<sample.size(); ++i) sample(i) = dist(rng);
}

void sampleNormal(boost::numeric::ublas::vector<double> &sample,
  const std::vector<double> &pars) {
  rnd::normal_distribution<> dist(pars[0], pars[1]);

  for (unsigned long int i=0; i<sample.size(); ++i) sample(i) = dist(rng);
}

void sampleStudent(boost::numeric::ublas::vector<double> &sample,
  const std::vector<double> &pars) {
  rnd::student_t_distribution<> dist(pars[0]);

  for (unsigned long int i=0; i<sample.size(); ++i) sample(i) = dist(rng);
}
