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

#include "options_parser.h"

namespace al = boost::algorithm;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

using namespace boost::assign;

template<typename T>
void validate(boost::any &v, const std::vector<std::string> &values,
  OptionVector<T> *, int) {
  po::validators::check_first_occurrence(v);

  std::vector<T> vec;
  OptionVector<T> optionVec;

  for (std::vector<std::string>::const_iterator i=values.begin();
    i!=values.end(); ++i) {
    std::stringstream sStream(*i);
    std::copy(std::istream_iterator<T>(sStream), std::istream_iterator<T>(),
      std::back_inserter(vec));
    if (!sStream.eof()) {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
  }

  std::copy(vec.begin(), vec.end(), std::back_inserter(optionVec.value));
  v = optionVec;
}

template<typename T>
void validate(boost::any &v, const std::vector<std::string> &values,
  OptionVectorGtor<T> *, int) {
  po::validators::check_first_occurrence(v);

  std::vector<T> vecGtor;
  OptionVectorGtor<T> optionVecGtor;

  for (std::vector<std::string>::const_iterator i=values.begin();
    i!=values.end(); ++i) {
    std::stringstream Sstream(*i);
    std::copy(std::istream_iterator<T>(Sstream), std::istream_iterator<T>(),
      std::back_inserter(vecGtor));
    if (!Sstream.eof()) {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
  }

  std::copy(vecGtor.begin(), vecGtor.end(),
    std::back_inserter(optionVecGtor.args));
  optionVecGtor.generateOptionSequence();
  v = optionVecGtor;
}

template<typename T>
void validate(boost::any &v, const std::vector<std::string> &values,
  OptionPositiveVectorGtor<T> *, int) {
  po::validators::check_first_occurrence(v);

  std::vector<T> vecGtor;
  OptionPositiveVectorGtor<T> optionPositiveVecGtor;

  for (std::vector<std::string>::const_iterator i=values.begin();
    i!=values.end(); ++i) {
    std::stringstream Sstream(*i);
    std::copy(std::istream_iterator<T>(Sstream), std::istream_iterator<T>(),
      std::back_inserter(vecGtor));
    if (!Sstream.eof()) {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
  }

  std::copy(vecGtor.begin(), vecGtor.end(),
    std::back_inserter(optionPositiveVecGtor.args));
  optionPositiveVecGtor.checkArgs();
  optionPositiveVecGtor.generateOptionSequence();
  v = optionPositiveVecGtor;
}

Options::Options(const int argc, const char *argv[]) :
  argc_(argc), argv_(argv), optionsPathHandler_(fs::path()) {
  modeArgs_ += "sim-ber", "sim-spr";

  verboseArgs_ += 1, 2;

  sysArgs_ += "csk-coh-su", "csk-ncoh-su";

  sprArgs_ += "bernoulli", "circular", "gumbel", "logistic",
    "negbernoulli", "normal", "pbcs", "student", "tent", "valley";
}

void Options::checkOptionConflict(const std::string opt1,
  const std::string opt2, const std::string msg) const {
  if (vm_.count(opt1) && !vm_[opt1].defaulted() &&
    vm_.count(opt2) && !vm_[opt2].defaulted()) {
    if (msg.empty()) {
      throw OptionConflict(std::string("Conflicting options \"") + opt1 +
        "\" and \"" + opt2 + "\"");
    }
    else throw OptionConflict(msg);
  }
}

void Options::checkOptionDependency(const std::string opt1,
  const std::string opt2, const std::string msg) const {
  if (vm_.count(opt1) && !vm_[opt1].defaulted() && !vm_.count(opt2)) {
    if (msg.empty()) {
      throw OptionDependency(std::string("Option \"") + opt1 +
        "\" requires option \"" + opt2 + "\"");
    }
    else throw OptionDependency(msg);
  }
}

std::string Options::serializeOptionValue(const po::variable_value &vv) {
  std::string s;

  if (!vv.empty()) {
    const std::type_info &type = vv.value().type();
    if (type==typeid(std::string)) {
      s = vv.as<std::string>();
    }
    else if (type==typeid(fs::path)) {
      s = boost::any_cast<fs::path>(vv.value()).generic_string();
    }
    else if (type==typeid(int)) {
      s = boost::lexical_cast<std::string>(boost::any_cast<int>(vv.value()));
    }
    else if (type==typeid(long int)) {
      s = boost::lexical_cast<std::string>(
        boost::any_cast<long int>(vv.value()));
    }
    else if (type==typeid(double)) {
      s = boost::lexical_cast<std::string>(boost::any_cast<double>(vv.value()));
    }
    else if (type==typeid(OptionVector<double>)) {
      s = serializeSequence(
        boost::any_cast<OptionVector<double> >(vv.value()).value);
    }
    else if (type==typeid(OptionPositiveVectorGtor<int>)) {
      s = serializeSequence(
        boost::any_cast<OptionPositiveVectorGtor<int> >(vv.value()).args);
    }
    else if (type==typeid(OptionVectorGtor<double>)) {
      s = serializeSequence(
        boost::any_cast<OptionVectorGtor<double> >(vv.value()).args);
    }
    else if (type==typeid(OptionPositiveVectorGtor<double>)) {
      s = serializeSequence(
        boost::any_cast<OptionPositiveVectorGtor<double> >(vv.value()).args);
    }
  }

  return s;
}

void Options::createOptionsScript() {
  for(po::variables_map::const_iterator i=vm_.begin(); i!=vm_.end(); ++i) {
    if (i->first=="script") {
      continue;
    }
    else if (i->first=="out-col-delim") {
      std::string s = serializeOptionValue(i->second);
      s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
      script += i->first + " = \"" + s + "\"\n";
    }
    else if (i->first=="mcml-tol") {
      if (vm_.count("dec") && dec=="mcml") {
        script += i->first + " = " + serializeOptionValue(i->second) + "\n";
      }
    }
    else if (i->first=="spr-nseq") {
      if(mode=="sim-spr") {
        script += i->first + " = " + serializeOptionValue(i->second) + "\n";
      }
    }
    else {
      script += i->first + " = " + serializeOptionValue(i->second) + "\n";
    }
  }
}

int Options::parseOptions() {
  try {
    po::options_description genericOptions("Generic options");
    genericOptions.add_options()
      ("version", "Print version number.\n")
      ("help,h", "Print usage message.\n")
      ("script,s",
        po::value<fs::path>(&optionsFile)->implicit_value("options.sbc"),
        "Read program options from the file indicated by -s or --script.");

    po::options_description modeOption("Mode (required option)");
    modeOption.add_options()
      ("mode,m", po::value<std::string>(&mode)->default_value("sim-ber"),
        "Specify run mode. Required option.\n"
        "Allowed values:\n"
        "   sim-ber: \tRun BER simulation.\n"
        "   sim-spr: \tGenerate spreading.");

    po::options_description ioOptions("Input-output options");
    ioOptions.add_options()
      ("out,o", po::value<fs::path>(&outputRoot)->
        default_value("./output"),
        "Specify output root filename.\n")
      ("out-col-delim",
        po::value<std::string>(&outputColDelim)->default_value("\t"),
        "Specify output column delimiter.\n")
      ("verbose,v", po::value<int>(&verbose)->default_value(1),
        "Specify verbosity level.\n"
        "Allowed values:\n"
        "  1: \tExecution runs with succinct verbosity.\n"
        "  2: \tExecution runs with detailed verbosity.");

    po::options_description simOptions("Simulation options");
    simOptions.add_options()
      ("seed", po::value<long int>(&seed),
        "Set the seed to an integer. If not set by the user, the seed is set "
        "by the calendar time.\n")
    ("n-ber", po::value<long int>(&nBerOption_),
      "Specify number of iterations for simulating a BER point.")
    ("n-mcml", po::value<long int>(&nMcmlOption_),
      "Specify number of spreading samples for simulating the MCML decoder.");

    po::options_description sysOptions("System options");
    sysOptions.add_options()
      ("sys", po::value<std::string>(&sys),
        "Specify communication system.\n"
        "Allowed values:\n"
        "   csk-coh-su: \tSingle-user coherent CSK.\n"
        "  csk-ncoh-su: \tSingle-user non-coherent CSK.")
    ("bit", po::value<int>(&bit),
      "Specify binary bit (information).\n")
    ("nse", po::value<std::string>(&nse),
      "Specify type of channel noise.\n"
      "Allowed values:\n"
      "  white: \tWhite noise.\n")
    ("nse-var",
      po::value<OptionPositiveVectorGtor<double> >(&nseVarGtor_)->
      multitoken(),
      "Specify channel noise variance.\n")
    ("snr-per-bit", po::value<OptionVectorGtor<double> >(&snrPerBitGtor_)->
      multitoken(),
      "Specify SNR.\n")
    ("nse-pars", po::value<OptionVector<double> >(&nseParsOptionVector_)->
      multitoken(),
      "Specify channel noise parameters.\n")
    ("dec", po::value<std::string>(&dec),
      "Specify decoder.\n"
      "Allowed values:\n"
      "  corr: \tCorrelation decoder.\n"
      "  mcml: \tMCML decoder.\n")
    ("mcml-brkt", po::value<OptionVector<double> >(&mcmlBrktPars_),
      "Specify lower and mid-abscissa for bracketing the MCML maximum. "
      "Omit the option for automatic bracketing.\n")
    ("mcml-tol", po::value<double>(&mcmlTol)->default_value(1e-10),
      "Specify fractional precision for the isolation of the MCML maximum.\n")
    ("ber-lb", po::value<std::string>(&berLb),
      "Specify type of BER lower bound to be calculated.\n"
      "Allowed values:\n"
      "  jensen: \tBER lower bound based on Jensen's inequality.\n");

    po::options_description sprOptions("Spreading options");
    sprOptions.add_options()
      ("spr", po::value<std::string>(&spr),
        "Specify type of spreading to be generated.\n"
        "Allowed values:\n"
        "     bernoulli: "
        "\tSpreading generated iteratively by the non-central Bernoulli map.\n"
        "      circular: "
        "\tSpreading generated iteratively by the circular map.\n"
        "        gumbel: \tGumbel (extreme value) spreading.\n"
        "      logistic: "
        "\tSpreading generated iteratively by the logistic map.\n"
        "  negbernoulli: "
        "\tSpreading generated iteratively by the non-central negative "
        "Bernoulli map.\n"
        "        normal: \tNormal spreading.\n"
        "          pbcs: Paired Bernoulli circular spreading.\n"
        "       student: \tStudent spreading.\n"
        "          tent: \tSpreading generated iteratively by the non-central "
        "tent map.\n"
        "        valley: "
        "\tSpreading generated iteratively by the non-central valley map.\n")
      ("spr-len", po::value<OptionPositiveVectorGtor<int> >(&sprLenGtor_)->
        multitoken(),
        "Specify spreading length.\n"
        "Ways to set the spreading length:\n"
        "             n: \tspr-len takes a single user-defined value n.\n"
        "       min max: \tspr-len is a sequence from min to max with step 1.\n"
        "  min max step: "
        "\tspr-len is a sequence from min to max with user-defined step.\n")
      ("spr-pars",
        po::value<OptionVector<double> >(&sprParsOptionVector_)->
        multitoken(),
        "Specify spreading parameters.\n")
      ("spr-nseq", po::value<long int>(&sprNSeqOption_)->default_value(1),
        "Specify number of spreading sequences to be generated.");

    po::options_description commandLineOptions;
    commandLineOptions.add(genericOptions).add(modeOption).add(ioOptions).
      add(simOptions).add(sysOptions).add(sprOptions);

    po::options_description configFileOptions;
    configFileOptions.add(modeOption).add(ioOptions).add(simOptions).
      add(sysOptions).add(sprOptions);

    po::parsed_options parsedOptions =
      po::command_line_parser(argc_, argv_).options(commandLineOptions).run();
    store(parsedOptions, vm_);

    if (vm_.count("version")) {
      std::cout << "CSK, Version 1.0" << std::endl;
      return 2;
    }
    if (vm_.count("help")) {
      std::cout << commandLineOptions << std::endl;
      return 2;
    }

    notify(vm_);

    if (vm_.count("script")) {
      optionsPathHandler_.setPath(optionsFile);
      optionsPathHandler_.isRegularFile();

      fs::ifstream optionsIfstream(optionsFile);

      store(parse_config_file(optionsIfstream, configFileOptions), vm_);
      notify(vm_);
    }

    inOptionArguments(modeArgs_, mode,
      std::string("\"mode\" was set to non-existing type \"") + mode + "\"");

    optionsPathHandler_.setPath(outputRoot);
    optionsPathHandler_.isRootFilename();

    outputColDelim.erase(std::remove(outputColDelim.begin(),
      outputColDelim.end(), '\"'), outputColDelim.end());

    inOptionArguments(verboseArgs_, verbose,
      "\"verbose\" was set to non-existing value \"" +
      boost::lexical_cast<std::string>(verbose) + "\"");

    if (vm_.count("seed")) {
      if (!al::starts_with(mode, "sim")) {
        throw OptionConflict("The selected mode \"" + mode +
          "\" does not require specifying the \"seed\" option");
      }
    }
    else {
      if (al::starts_with(mode, "sim")) {
        seed = static_cast<long int>(std::time(NULL));
      }
    }

    if (vm_.count("n-ber")) {
      if (mode=="sim-ber") {
        if (isPositive(nBerOption_)) {
          nBer = static_cast<unsigned long int>(nBerOption_);
        }
        else {
          throw InvalidSign("\"n-ber\" must be a positive integer");
        }
      }
      else {
        throw OptionConflict("The selected mode \"" + mode +
          "\" does not require specifying the \"n-ber\" option");
      }
    }
    else {
      if (mode=="sim-ber") {
        throw OptionDependency("The selected mode \"" + mode +
          "\" requires specifying the \"n-ber\" option");
      }
    }

    if (vm_.count("sys")) {
      if (mode=="sim-ber") {
        inOptionArguments(sysArgs_, sys,
          "\"sys\" was set to non-existing type \"" + sys + "\"");
      }
      else {
        throw OptionConflict("The selected mode \"" + mode +
          "\" does not require specifying the \"sys\" option");
      }
    }
    else {
      if (mode=="sim-ber") {
        throw OptionDependency("The selected mode \"" + mode +
          "\" requires specifying the \"sys\" option");
      }
    }

    checkOptionDependency("bit", "sys");

    if (vm_.count("bit")) {
      if (vm_.count("sys")) {
        if (sys=="csk-coh-su" || sys=="csk-ncoh-su") {
          if (bit!=-1 && bit!=1) {
            throw NotElement("\"bit\" was set to non-existing value \"" +
              boost::lexical_cast<std::string>(bit) +
              "\" for the chosen system \"" + sys + "\"");
          }
        }
        else {
          throw OptionConflict("The selected system \"" + sys +
            "\" does not require specifying the \"bit\" option");
        }
      }
    }
    else {
      if (vm_.count("sys")) {
        if (sys=="csk-coh-su" || sys=="csk-ncoh-su") {
          throw OptionDependency("The selected system \"" + sys +
            "\" requires specifying the \"bit\" option");
        }
      }
    }

    checkOptionDependency("nse", "sys");
    checkOptionDependency("sys", "nse");
    checkOptionDependency("nse-var", "nse");
    checkOptionDependency("snr-per-bit", "nse");
    checkOptionDependency("nse-pars", "nse");

    if (vm_.count("nse")) {
      if (vm_.count("sys")) {
        if (sys=="csk-coh-su" || sys=="csk-ncoh-su") {
          if (nse!="white") {
            throw NotElement("\"nse\" was set to non-existing type \"" + nse +
              "\" for the chosen system \"" + sys + "\"");
          }
        }
      }

      if (vm_.count("nse-var") && !vm_.count("snr-per-bit")) {
        std::copy(nseVarGtor_.value.begin(), nseVarGtor_.value.end(),
          std::back_inserter(nseVar));
      }
      else if (!vm_.count("nse-var") && vm_.count("snr-per-bit")){
        std::copy(snrPerBitGtor_.value.begin(), snrPerBitGtor_.value.end(),
          std::back_inserter(snrPerBit));
      }
      else if (vm_.count("nse-var") && vm_.count("snr-per-bit")){
        throw OptionConflict(
          "Conflicting options \"nse-var\" and \"snr-per-bit\"");
      }
      else if (!vm_.count("nse-var") && !vm_.count("snr-per-bit")){
        throw OptionDependency(std::string("\"nse\" requires either ") +
          "option \"nse-var\" or \"snr-per-bit\"");
      }

      if (vm_.count("nse-pars")) {
        std::copy(nseParsOptionVector_.value.begin(),
          nseParsOptionVector_.value.end(), std::back_inserter(nsePars));
      }
    }

    checkOptionDependency("dec", "sys");
    checkOptionDependency("sys", "dec");
    checkOptionDependency("n-mcml", "dec");
    checkOptionDependency("mcml-brkt", "dec");
    checkOptionDependency("mcml-tol", "dec");

    if (vm_.count("dec")) {
      if (vm_.count("sys")) {
        if ((sys=="csk-coh-su" && dec!="corr") ||
          (sys=="csk-ncoh-su" && dec!="corr" && dec!="mcml")) {
          throw NotElement("\"dec\" was set to non-existing type \"" + dec +
            "\" for the chosen system \"" + sys + "\"");
        }
      }

      if (dec=="mcml") {
        if (vm_.count("n-mcml")) {
          if (isPositive(nMcmlOption_)) {
            nMcml = static_cast<unsigned long int>(nMcmlOption_);
          }
          else {
            throw InvalidSign("\"n-mcml\" must be a positive integer");
          }
        }
        else {
          throw OptionDependency("The selected decoder \"" + dec +
            "\" requires specifying the \"n-mcml\" option");
        }

        if (vm_.count("mcml-brkt")) {
          if (mcmlBrktPars_.value.size()==2) {
            mcmlDefaultBrkt = false;

            std::copy(mcmlBrktPars_.value.begin(), mcmlBrktPars_.value.end(),
              std::back_inserter(mcmlBrktPars));
          }
          else {
            throw InvalidIndex(std::string("The bracketing of the MCML ") +
              "maximum requires exactly two arguments in the \"mcml-brkt\" " +
              "option, namely the lower and mid-abscissa of the bracket.");
          }
        }
        else {
          mcmlDefaultBrkt = true;
        }

        if (vm_.count("mcml-tol") && !vm_["mcml-tol"].defaulted()) {
          if (!isPositive(mcmlTol)) {
            throw InvalidSign("\"mcml-tol\" must be a positive real number");
          }
        }
      }
      else {
        if (vm_.count("n-mcml")) {
          throw OptionConflict("The selected decoder \"" + dec +
            "\" does not require specifying the \"n-mcml\" option");
        }

        if (vm_.count("mcml-brkt")) {
          throw OptionConflict("The selected decoder \"" + dec +
            "\" does not require specifying the \"mcml-brkt\" option");
        }

        if (vm_.count("mcml-tol") && !vm_["mcml-tol"].defaulted()) {
          throw OptionConflict("The selected decoder \"" + dec +
            "\" does not require specifying the \"mcml-tol\" option");
        }
      }
    }

    checkOptionDependency("ber-lb", "sys");

    if (vm_.count("ber-lb")) {
      if (vm_.count("sys")) {
        if (sys=="csk-coh-su") {
          if (berLb!="jensen") {
            throw NotElement("\"ber-lb\" was set to non-existing type \"" +
              berLb + "\" for the chosen system \"" + sys + "\"");
          }
        }
        else if (sys=="csk-ncoh-su") {
          throw OptionConflict("The selected sys \"" + sys +
            "\" does not require specifying the \"ber-lb\" option");
        }
      }
    }

    checkOptionDependency("spr-len", "spr");
    checkOptionDependency("spr", "spr-len");
    checkOptionDependency("spr-pars", "spr");
    checkOptionDependency("spr-nseq", "spr");

    if (vm_.count("spr")) {
      if (mode=="sim-spr" ||
        (mode=="sim-ber" &&
        (sys=="csk-coh-su" || sys=="csk-ncoh-su"))) {
        inOptionArguments(sprArgs_, spr,
          "\"spr\" was set to non-existing type \"" + spr + "\"");
      }

      if (vm_.count("spr-len")) {
        std::copy(sprLenGtor_.value.begin(), sprLenGtor_.value.end(),
          std::back_inserter(sprLen));
      }

      if (vm_.count("spr-pars")) {
        std::copy(sprParsOptionVector_.value.begin(),
          sprParsOptionVector_.value.end(), std::back_inserter(sprPars));
      }

      if (mode=="sim-spr") {
        if (isPositive(sprNSeqOption_)) {
          sprNSeq = static_cast<unsigned long int>(sprNSeqOption_);
        }
        else {
          throw InvalidSign("\"spr-nseq\" must be a positive integer");
        }
      }
    }
    else {
      if (mode=="sim-spr" ||
        (mode=="sim-ber" &&
        (sys=="csk-coh-su" || sys=="csk-ncoh-su"))) {
          throw OptionDependency(
            "\"spr\" option required under the given options");
      }
    }

    createOptionsScript();
  }
  catch(po::error &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  catch(LogicError &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }

  return 0;
}
