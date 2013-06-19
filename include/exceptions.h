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

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>

class LogicError : public std::logic_error {
public:
  explicit LogicError(const std::string &);
};

class InvalidType : public LogicError {
  public:
    explicit InvalidType(const std::string &);
};

class NonExistent : public LogicError {
  public:
    explicit NonExistent(const std::string &);
};

class NotElement : public LogicError {
  public:
    explicit NotElement(const std::string &);
};

class NumberError : public LogicError {
  public:
    explicit NumberError(const std::string &);
};

class DivisionByZero : public NumberError {
  public:
    explicit DivisionByZero(const std::string &);
};

class InvalidSign : public NumberError {
  public:
    explicit InvalidSign(const std::string &);
};

class OptionError : public LogicError {
  public:
    explicit OptionError(const std::string &);
};

class OptionConflict : public OptionError {
  public:
    explicit OptionConflict(const std::string &);
};

class OptionDependency : public OptionError {
  public:
    explicit OptionDependency(const std::string &);
};

class SequenceError : public LogicError {
  public:
    explicit SequenceError(const std::string &);
};

class InvalidIndex : public SequenceError {
  public:
    explicit InvalidIndex(const std::string &);
};

class InvalidOrder : public SequenceError {
  public:
    explicit InvalidOrder(const std::string &);
};

class RuntimeError : public std::runtime_error {
public:
  explicit RuntimeError(const std::string &);
};

class MinMaxError : public RuntimeError {
  public:
    explicit MinMaxError(const std::string &);
};

#endif /* EXCEPTIONS_H_ */
