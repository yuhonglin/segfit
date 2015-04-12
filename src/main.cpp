/**
 * @file   main.cpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Sun Dec 22 10:39:41 2013
 *
 * @brief  SegFit: an algorithm combining segmentation and fitting
 *
 *
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <list>
#include <string>
#include <sstream>

#include <cstdlib>
#include <cmath>

#ifdef DEBUG
#include <fenv.h>
#endif

#include "option.hpp"
#include "logger.hpp"
#include "SegAlgFactory.hpp"
#include "SegAlg.hpp"

using namespace std;

int main(int argc, char *argv[]) {

#ifdef DEBUG
  /**
   * function (log, pow) in cmath may lead to FE_INEXACT exceptions
   */
  feenableexcept(-1 xor FE_INEXACT);
#endif

  LOGGER->enable_exception = false;

  /* parse options */
  dms::Option opt;

  opt.add_string("-alg", "algorithm type", "dp",
                 set<string>({ "dp", "topdown" }));
  opt.add_string("-fit", "fitting type", "powerabc",
                 set<string>({ "powerabc", "abline", "const" }));
  opt.add_string("-s", "sequence to b e segmented");
  opt.add_int("-n", "first n numbers", -1);

  switch (opt.parse(argc, argv)) {
  case dms::Option::PARSE_HELP: { return 0; }
  case dms::Option::PARSE_ERROR: {
    { LOG_ERROR("option parsing error, use --help to for usage"); }
    return 1;
  }
  default: { break; }
  }

  string algtype = opt.get_string("-alg");
  string fittype = opt.get_string("-fit");
  string sequence = opt.get_string("-s");
  int firstN = opt.get_int("-n");

  // convert the input string into a double array
  shared_ptr<vector<double> > seqdbptr(new vector<double>);
  sequence.erase(sequence.find_last_not_of(" \n\r\t") + 1);

  for (unsigned int i = 0; i < sequence.size(); i++) {
    if (sequence[i] == ',' or sequence[i] == '\t' or sequence[i] ==
        '\n' or sequence[i] == ' ') {
      sequence[i] = ' ';
      continue;
    }

    if (sequence[i] > '9' or sequence[i] < '0') {
      stringstream ss;
      ss << "invalid charactor \"" << sequence[i]
         << "\" in the input sequence at position " << i
         << ", check the value after \"-s\" option";
      LOG_ERROR(ss.str());
      exit(1);
    }
  }

  stringstream ss(sequence);

  double tmp;
  while (!ss.eof()) {
    ss >> tmp;
    seqdbptr->push_back(tmp);
  }

  if (firstN > static_cast<int>(seqdbptr->size())) {
    LOG_WARNING("input sequence is shorter than expected");
    firstN = seqdbptr->size();
  } else if (firstN <= 0) {
    firstN = seqdbptr->size();
  }

  // compute

  SegAlgFactory factory;
  unique_ptr<SegAlg> segalg = factory.make(algtype);

  segalg->set_fitalg(fittype);

  segalg->set_string(seqdbptr);

  segalg->run();

  segalg->dump();

  return 0;
}
