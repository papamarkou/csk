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

#include "csk.h"

namespace al = boost::algorithm;

int main(const int argc, const char *argv[]) {
  int exitStatus;

  Options sbcOptions(argc, argv);

  boost::filesystem::path errorFile;

  boost::filesystem::path logFile;

  boost::filesystem::ofstream optionsOfstream;
  boost::filesystem::path optionsFile;

  std::istringstream scriptStream;
  std::string scriptLine;

  Copyright sbcCopyright;
  sbcCopyright.software="CSK";
  sbcCopyright.version="1.0";
  sbcCopyright.author="Theodore Papamarkou";
  sbcCopyright.date="1 Jan 2013";
  sbcCopyright.license="GNU General Public License, version 3";
  sbcCopyright.website="http://www.csk.org";

  Timer timer;

  exitStatus = sbcOptions.parseOptions();
  if (exitStatus!=0) return 1;

  errorFile = sbcOptions.outputRoot;
  errorFile = errorFile.replace_extension(".error");
  errorOfstream.open(errorFile);

  logFile = sbcOptions.outputRoot;
  logFile = logFile.replace_extension(".log");
  logOfstream.open(logFile);

  optionsFile = sbcOptions.outputRoot;
  optionsFile = optionsFile.replace_extension(".sbc");

  logTee << "\n" + std::string(80, '*') + "\n|" +
    std::string(11, ' ') + sbcCopyright.software + std::string(6, ' ') +
    "|" + std::string(6, ' ') + "version " + sbcCopyright.version +
    std::string(6, ' ') + "|" + std::string(6, ' ') + sbcCopyright.date +
    std::string(11, ' ') + "|\n|" +
    std::string(78, '-') + "|\n|" +
    std::string(20, ' ') + "Copyright (C) 2013 " + sbcCopyright.author +
    std::string(20, ' ') + "|\n|" +
    std::string(78, '-') + "|\n|" +
    std::string(9, ' ') + "Distributed under the " + sbcCopyright.license +
    "." + std::string(9, ' ') + "|\n|" +
    std::string(78, '-') + "|\n|" +
    std::string(27, ' ') + sbcCopyright.website + std::string(27, ' ') + "|\n" +
    std::string(80, '*') + "\n\n";

  if (sbcOptions.verbose==2) logTee << "Options parsed successfully\n\n";

  timer.start();

  logTee << "Program execution is starting at " <<
    convertChronoTimeToString(timer.getStartTime()) << "\n\n";

  if (al::starts_with(sbcOptions.mode, "sim")) {
    rng.seed(sbcOptions.seed);
    logTee << "Seed set to " << sbcOptions.seed << "\n\n";
  }

  logTee.flush();

  if (sbcOptions.mode=="sim-ber") {
    exitStatus = simulateBer(sbcOptions);
    if (exitStatus!=0) return 1;
  }
  else if (sbcOptions.mode=="sim-spr") {
    exitStatus = simulateSpreading(sbcOptions);
    if (exitStatus!=0) return 1;
  }

  timer.stop();

  logTee << "Program execution completed at " <<
    convertChronoTimeToString(timer.getStopTime()) << "\n";

  logTee << "CSK's execution time: "
    + displaySecondsAsDays(timer.getTotalEllapsedTime()) + "\n\n";

  logTee << "The following CSK script was executed:\n";
  scriptStream.str(sbcOptions.script);
  while (std::getline(scriptStream, scriptLine)) {
    logTee << "  " << scriptLine << "\n";
  }

  optionsOfstream.open(optionsFile);
  optionsOfstream << sbcOptions.script;
  optionsOfstream.close();

  exitTeeDevices();

  return 0;
}
