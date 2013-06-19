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

#ifndef TEE_DEVICES_H_
#define TEE_DEVICES_H_

#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

typedef boost::iostreams::tee_device<std::ostream,
  boost::filesystem::ofstream> TeeDevice;
typedef boost::iostreams::stream<TeeDevice> TeeStream;

extern boost::filesystem::ofstream errorOfstream;
extern boost::filesystem::ofstream logOfstream;

extern const TeeDevice errorTeeDevice;
extern const TeeDevice logTeeDevice;

extern TeeStream errorTee;
extern TeeStream logTee;

void exitTeeDevices();

#endif /* TEE_DEVICES_H_ */
