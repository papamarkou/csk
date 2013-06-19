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

#ifndef TIMING_H_
#define TIMING_H_

#include <boost/chrono/chrono_io.hpp>
#include <boost/lexical_cast.hpp>

template <class R, class P>
std::string displaySecondsAsDays(const boost::chrono::duration<R, P> d)
{
  typedef boost::chrono::duration<long long, boost::centi> CentiSeconds;
  typedef boost::chrono::duration<long long, boost::ratio<86400> > Days;

  std::string ellapsedTime_;
  CentiSeconds centiSeconds = boost::chrono::duration_cast<CentiSeconds>(d);
  Days days;
  boost::chrono::seconds seconds;
  boost::chrono::minutes minutes;
  boost::chrono::hours hours;

  if (d - centiSeconds > boost::chrono::milliseconds(5) ||
    (d - centiSeconds == boost::chrono::milliseconds(5) &&
    (centiSeconds.count() & 1))) ++centiSeconds;
  seconds = boost::chrono::duration_cast<boost::chrono::seconds>(centiSeconds);
  centiSeconds -= seconds;
  minutes = boost::chrono::duration_cast<boost::chrono::minutes>(seconds);
  seconds -= minutes;
  hours = boost::chrono::duration_cast<boost::chrono::hours>(minutes);
  minutes -= hours;
  days = boost::chrono::duration_cast<Days>(hours);
  hours -= days;

  if (days < Days(100)) {
    if (days < Days(10)) ellapsedTime_ += "00";
    else ellapsedTime_ += "0";
  }
  ellapsedTime_ += boost::lexical_cast<std::string>(days.count()) + "/";
  if (hours < boost::chrono::hours(10)) ellapsedTime_ += "0";
  ellapsedTime_ += boost::lexical_cast<std::string>(hours.count()) + ":";
  if (minutes < boost::chrono::minutes(10)) ellapsedTime_ += "0";
  ellapsedTime_ += boost::lexical_cast<std::string>(minutes.count()) + ":";
  if (seconds < boost::chrono::seconds(10)) ellapsedTime_ += "0";
  ellapsedTime_ += boost::lexical_cast<std::string>(seconds.count()) + ".";
  if (centiSeconds < CentiSeconds(10)) ellapsedTime_ += "0";
  ellapsedTime_ += boost::lexical_cast<std::string>(centiSeconds.count());

  return ellapsedTime_;
}

class Timer {
    boost::chrono::system_clock::time_point baseTime_, startTime_, stopTime_;

    boost::chrono::duration<double> ellapsedTime_, totalEllapsedTime_;
  public:
    Timer();

    void start();
    void stop();

    boost::chrono::system_clock::time_point getBaseTime();
    boost::chrono::system_clock::time_point getStartTime();
    boost::chrono::system_clock::time_point getStopTime();

    boost::chrono::duration<double> getEllapsedTime();
    boost::chrono::duration<double> getTotalEllapsedTime();
};

std::string convertChronoTimeToString(
  const boost::chrono::system_clock::time_point);

#endif /* TIMING_H_ */
