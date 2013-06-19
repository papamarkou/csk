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

#include "file_handler.h"

namespace fs = boost::filesystem;

PathHandler::PathHandler(const fs::path p) : path_(p) {}

void PathHandler::setPath(const fs::path p) {
  path_ = p;
}

void PathHandler::isRegularFile() const {
  if (exists(path_)) {
    if (!is_regular_file(path_)) {
      if (is_directory(path_)) {
        throw InvalidType(std::string("Error in ") +
          "\"PathHandler::isRegularFile\":\n  " +
          path_.generic_string() +
          "\n  is a directory, not a file.");
      }
      else {
        throw InvalidType(std::string("Error in ") +
          "\"PathHandler::isRegularFile\":\n  " +
          path_.generic_string() +
          "\n  exists but it is neither a regular file nor a directory.");
      }
    }
  }
  else {
    throw NonExistent(std::string("Error in ") +
      "\"PathHandler::isRegularFile\":\n  " +
      path_.generic_string() +
      "\n  does not exist.");
  }
}

void PathHandler::isRootFilename() const {
  if (is_directory(path_)) {
    throw InvalidType(std::string("Error in ") +
      "\"PathHandler::isRootFilename\":\n  " +
      path_.generic_string() +
      "\n  is a directory, not a root filename.");
  }
  else {
    if (!is_directory(path_.parent_path())) {
      throw NonExistent(std::string("Error in ") +
        "\"PathHandler::isRootFilename\":\n  " +
        "The parent path of the output root filename is\n  " +
        path_.parent_path().generic_string() +
        "\n  This is not an existing directory.");
    }
  }
}
