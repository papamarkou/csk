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

#ifndef SEQUENCE_ALGORITHMS_H_
#define SEQUENCE_ALGORITHMS_H_

#include <algorithm>
#include <deque>
#include <list>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "exceptions.h"
#include "number_algorithms.h"

template <typename T, template <class, class> class C>
bool inSequence(C<T, std::allocator<T> > sequence, const T element) {
  std::sort(sequence.begin(), sequence.end());

  return std::binary_search(sequence.begin(), sequence.end(), element);
}

template<typename T>
bool nElementsLessOrEqual(const std::vector<T> sequence,
  const unsigned int nElements) {
  return (sequence.size()<=nElements);
}

template <typename T, template <class, class> class C>
void generateSequence(const T start, const T end, const T by,
  C<T, std::allocator<T> > &sequence) {
    if (!isPositive<T>(by)) {
      throw InvalidSign(std::string("Error in \"generateSequence\":\n  ") +
        "The step of the sequence must be specified as a positive number");
    }

    T step = start;

  if (start<end) {
    while(step<=end) {
      sequence.insert(sequence.end(), step);
      step += by;
    }
  }
  else if (start>end) {
    while(step>=end) {
      sequence.insert(sequence.end(), step);
      step -= by;
    }
  }
  else {
    sequence.insert(sequence.end(), step);
  }
}

template <typename T>
std::string serializeSequence(const T &sequence,
  const std::string separator=" ") {
  std::string output;
  typename T::const_iterator pos;

  for (pos=sequence.begin(); pos!=sequence.end(); ++pos) {
    output += boost::lexical_cast<std::string>(*pos) +
      ((pos!=--sequence.end()) ? separator : "");
  }

  return output;
}

template <typename T>
void printSequence(const T &sequence, const std::string separator=" ") {
  typename T::const_iterator pos;

  for (pos=sequence.begin(); pos!=sequence.end(); ++pos) {
    std::cout << *pos << ((pos!=--sequence.end()) ? separator : "");
  }
}

template <typename T>
void copyVectorToMatrixCol(const boost::numeric::ublas::vector<T> &vector,
  boost::numeric::ublas::matrix<T> &matrix, const unsigned long int j) {
  if (matrix.size1()!=vector.size())
    throw InvalidIndex(std::string("Error in \"copyVectorToMatrixCol\":\n  ") +
      "Matrix row and vector dimensions disagree");

  if (j>matrix.size2())
    throw InvalidIndex(std::string("Error in \"copyVectorToMatrixCol\":\n  ") +
      "Vector column location greater than the matrix's number of columns");

  for (unsigned long int i=0; i<matrix.size1(); ++i) {
    matrix(i, j) = vector(i);
  }
}

template <typename T1, typename T2>
void copyAnyTypeVectorToMatrixCol(
  const boost::numeric::ublas::vector<T1> &vector,
  boost::numeric::ublas::matrix<T2> &matrix, const unsigned long int j) {
  if (matrix.size1()!=vector.size())
    throw InvalidIndex(std::string("Error in ") +
      "\"copyAnyTypeVectorToMatrixCol\":\n  " +
      "Matrix row and vector dimensions disagree");

  if (j>matrix.size2())
    throw InvalidIndex(std::string("Error in ") +
      "\"copyAnyTypeVectorToMatrixCol\":\n  " +
      "Vector column location greater than the matrix's number of columns");

  for (unsigned long int i=0; i<matrix.size1(); ++i) {
    matrix(i, j) = static_cast<T2>(vector(i));
  }
}

#endif /* SEQUENCE_ALGORITHMS_H_ */
