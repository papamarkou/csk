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

#include "timing.h"

namespace chr = boost::chrono;

Timer::Timer() {
  baseTime_ = chr::system_clock::now();
}

void Timer::start() {
  startTime_ = chr::system_clock::now();
}

void Timer::stop() {
  stopTime_ = chr::system_clock::now();
}

chr::system_clock::time_point Timer::getBaseTime() {
  return baseTime_;
}

chr::system_clock::time_point Timer::getStartTime() {
  return startTime_;
}

chr::system_clock::time_point Timer::getStopTime() {
  return stopTime_;
}

chr::duration<double> Timer::getEllapsedTime() {
  ellapsedTime_ = stopTime_-startTime_;

  return ellapsedTime_;
}

chr::duration<double> Timer::getTotalEllapsedTime() {
  totalEllapsedTime_ = stopTime_-baseTime_;

  return totalEllapsedTime_;
}

std::string convertChronoTimeToString(
  const chr::system_clock::time_point chronoTime) {
  std::time_t timeT;
  char * ascTime;
  std::string stringTime;

  timeT = chr::system_clock::to_time_t(chronoTime);
  ascTime = std::asctime(std::localtime(&timeT));
  ascTime[strlen(ascTime)-1] = '\0';
  stringTime = std::string(ascTime);

  return stringTime;
}
