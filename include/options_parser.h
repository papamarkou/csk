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

#ifndef OPTIONS_PARSER_H_
#define OPTIONS_PARSER_H_

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "file_handler.h"
#include "number_algorithms.h"
#include "sequence_algorithms.h"

template<typename T>
struct OptionVector {
    std::vector<T> value;
};

template<typename T>
struct OptionVectorGtor {
    std::vector<T> value, args;

    void generateOptionSequence();
};

template <typename T>
void OptionVectorGtor<T>::generateOptionSequence() {
  if (args.size()==1) {
    value.push_back(args[0]);
  }
  else if (args.size()==2) {
    generateSequence(args[0], args[1], static_cast<T>(1), value);
  }
  else if (args.size()==3) {
    try {
      generateSequence(args[0], args[1], args[2], value);
    }
    catch(InvalidSign &error) {
      throw boost::program_options::validation_error(
        boost::program_options::validation_error::invalid_option_value);
    }
  }
  else {
    throw boost::program_options::validation_error(
      boost::program_options::validation_error::invalid_option_value);
  }
}

template<typename T>
struct OptionPositiveVectorGtor : public OptionVectorGtor<T> {
  void checkArgs() const;
};

template <typename T>
void OptionPositiveVectorGtor<T>::checkArgs() const {
  if (!boost::algorithm::all_of(this->args.begin(), this->args.end(),
    isPositive<T>)) {
    throw boost::program_options::validation_error(
      boost::program_options::validation_error::invalid_option_value);
  }
}

class Options {
    const int argc_;
    const char **argv_;

    boost::program_options::variables_map vm_;

    std::vector<std::string> modeArgs_;

    PathHandler optionsPathHandler_;
    std::vector<int> verboseArgs_;

    long int nBerOption_, nMcmlOption_;

    std::vector<std::string> sysArgs_;

    OptionPositiveVectorGtor<double> nseVarGtor_;
    OptionVectorGtor<double> snrPerBitGtor_;
    OptionVector<double> nseParsOptionVector_;

    OptionVector<double> mcmlBrktPars_;

    OptionPositiveVectorGtor<int> sprLenGtor_;
    OptionVector<double> sprParsOptionVector_;
    long int sprNSeqOption_;
    std::vector<std::string> sprArgs_;
  public:
    boost::filesystem::path optionsFile;

    std::string mode;

    boost::filesystem::path outputRoot;
    std::string outputColDelim;
    int verbose;

    long int seed;
    unsigned long int nBer, nMcml;

    std::string sys;

    int bit;

    std::string nse;
    std::vector<double> nseVar, snrPerBit, nsePars;

    std::string dec;
    bool mcmlDefaultBrkt;
    std::vector<double> mcmlBrktPars;
    double mcmlTol;

    std::string berLb;

    std::string spr;
    std::vector<int> sprLen;
    std::vector<double> sprPars;
    unsigned long int sprNSeq;

    std::string script;

    Options(const int, const char **);

    void checkOptionConflict(const std::string, const std::string,
      const std::string msg="") const;

    void checkOptionDependency(const std::string, const std::string,
      const std::string msg="") const;

    template <typename T, template <class, class> class C>
    void inOptionArguments(C<T, std::allocator<T> >, const T,
      const std::string) const;

    std::string serializeOptionValue(
      const boost::program_options::variable_value &);

    void createOptionsScript();

    int parseOptions();
};

template <typename T, template <class, class> class C>
void Options::inOptionArguments(C<T, std::allocator<T> > args, const T arg,
  const std::string msg) const {
  if (!inSequence(args, arg)) throw NotElement(msg);
}

#endif /* OPTIONS_PARSER_H_ */
