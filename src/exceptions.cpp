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

#include "exceptions.h"

LogicError::LogicError(const std::string &msg =
  "Logic error occurred.") : std::logic_error(msg) {}

InvalidType::InvalidType(const std::string &msg =
  "Provided entity of wrong type.") : LogicError(msg) {}

NonExistent::NonExistent(const std::string &msg =
  "Provided non-existent entity.") : LogicError(msg) {}

NotElement::NotElement(const std::string &msg =
  "Provided entity which is not an element of the set.") : LogicError(msg) {}

NumberError::NumberError(const std::string &msg =
  "Provided number with wrong sign.") : LogicError(msg) {}

DivisionByZero::DivisionByZero(const std::string &msg =
  "Divided by zero.") : NumberError(msg) {}

InvalidSign::InvalidSign(const std::string &msg =
  "Provided number with wrong sign.") : NumberError(msg) {}

OptionError::OptionError(const std::string &msg =
  "Error in parsing options.") : LogicError(msg) {}

OptionConflict::OptionConflict(const std::string &msg =
  "Options conflict occurred.") : OptionError(msg) {}

OptionDependency::OptionDependency(const std::string &msg =
  "Unmet option dependency.") : OptionError(msg) {}

SequenceError::SequenceError(const std::string &msg =
  "Indexed element out of the sequence's range.") : LogicError(msg) {}

InvalidIndex::InvalidIndex(const std::string &msg =
  "Indexed element out of the sequence's range.") : SequenceError(msg) {}

InvalidOrder::InvalidOrder(const std::string &msg =
  "Indexed element out of the sequence's range.") : SequenceError(msg) {}

RuntimeError::RuntimeError(const std::string &msg =
  "Runtime error occurred.") : std::runtime_error(msg) {}

MinMaxError::MinMaxError(const std::string &msg =
  "Minimization or maximization failed.") : RuntimeError(msg) {}
