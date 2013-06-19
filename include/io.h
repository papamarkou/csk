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

#ifndef IO_H_
#define IO_H_

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "sequence_algorithms.h"
#include "tee_devices.h"

template<typename T>
void printBoostVectorRow(const boost::numeric::ublas::vector<T> &vector,
  const std::string separator=" ") {
  for (unsigned long int i=0; i<vector.size(); ++i) {
    logTee << vector(i) << ((i!=(vector.size()-1)) ? separator : "\n");
  }
}

template<typename T>
void printBoostVectorCol(const boost::numeric::ublas::vector<T> &vector) {
  for (unsigned long int i=0; i<vector.size(); ++i) {
    logTee << vector(i) << "\n";
  }
}

template<typename T>
void printfBoostVectorRow(const boost::numeric::ublas::vector<T> &vector,
  const boost::filesystem::path file, const std::string separator=" ") {
  boost::filesystem::ofstream rowOfstream;
  rowOfstream.open(file);

  for (unsigned long int i=0; i<vector.size(); ++i) {
    rowOfstream << vector(i) << ((i!=(vector.size()-1)) ? separator : "\n");
  }

  rowOfstream.close();
}

template<typename T>
void printfBoostVectorCol(const boost::numeric::ublas::vector<T> &vector,
  const boost::filesystem::path file) {
  boost::filesystem::ofstream colOfstream;
  colOfstream.open(file);

  for (unsigned long int i=0; i<vector.size(); ++i) {
    colOfstream << vector(i) << "\n";
  }

  colOfstream.close();
}

template<typename T>
void printfBoostMatrix(boost::numeric::ublas::matrix<T> &matrix,
  boost::filesystem::path file, const std::string separator=" ",
  const std::string header="") {
  boost::filesystem::ofstream matOfstream;
  matOfstream.open(file);

  if (!header.empty()) matOfstream << header << "\n";
  for (unsigned long int i=0; i<matrix.size1(); ++i) {
    for (unsigned long int j=0; j<matrix.size2(); ++j) {
      matOfstream << matrix(i, j) <<
        ((j!=(matrix.size2()-1)) ? separator : "\n");
    }
  }

  matOfstream.close();
}

template<typename T>
void printfBoostMatrixCheckingNa(boost::numeric::ublas::matrix<T> &matrix,
  boost::filesystem::path file, const T na, const std::string naOut="NA",
  const std::string separator=" ", const std::string header="",
  const std::vector<unsigned long int> &uncheckedRows=
    std::vector<unsigned long int>(),
  const std::vector<unsigned long int> &uncheckedCols=
    std::vector<unsigned long int>()) {
  boost::filesystem::ofstream matOfstream;
  matOfstream.open(file);

  if (!uncheckedRows.empty() &&
    *std::max_element(uncheckedRows.begin(), uncheckedRows.end())>
    matrix.size1()) {
      throw InvalidIndex(std::string("Error in ") +
        "\"printfBoostMatrixCheckingNa\":\n  " +
        "Indices of rows unchecked for NA beyond matrix's row size");
    }

  if (!uncheckedCols.empty() &&
    *std::max_element(uncheckedCols.begin(), uncheckedCols.end())>
    matrix.size2()) {
      throw InvalidIndex(std::string("Error in ") +
        "\"printfBoostMatrixCheckingNa\":\n  " +
        "Indices of columns unchecked for NA beyond matrix's column size");
    }

  if (!header.empty()) matOfstream << header << "\n";
  for (unsigned long int i=0; i<matrix.size1(); ++i) {
    for (unsigned long int j=0; j<matrix.size2(); ++j) {
      if ((!uncheckedRows.empty() && inSequence(uncheckedRows, i+1)) ||
        (!uncheckedCols.empty() && inSequence(uncheckedCols, j+1))) {
        matOfstream << boost::lexical_cast<std::string>(matrix(i, j)) <<
          ((j!=(matrix.size2()-1)) ? separator : "\n");
      }
      else {
        matOfstream << ((matrix(i, j)!=na) ?
          boost::lexical_cast<std::string>(matrix(i, j)) : naOut) <<
          ((j!=(matrix.size2()-1)) ? separator : "\n");
      }
    }
  }

  matOfstream.close();
}

#endif /* IO_H_ */
