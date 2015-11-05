#include <algorithm>

#include "logger.hpp"
#include "SegAlg.hpp"

SegAlg::SegAlg()
    : _scale(100.0), _result(new vector<Segment>), _fitAlg(std::nullptr_t()),
      _name(""), _fitAlgFactory(new FitAlgFactory()) {}

SegAlg::~SegAlg() {}

void SegAlg::set_fitalg(string n) { _fitAlg = _fitAlgFactory->make(n); }

void SegAlg::set_string(shared_ptr<vector<double> > &pd) {
  if (_fitAlg != std::nullptr_t()) {
    _shrS = pd;
    _fitAlg->set_string(pd);

  } else {
    LOG_WARNING("not set fitting algorithm yet, can not set string.");
  }
}

/// parameter interface
void SegAlg::set_parameter(string n, double v) {
  if (n == "scale") {
    if (_fitAlg != std::nullptr_t()) {
      _fitAlg->set_parameter(n, v);
      _scale = v;
      return;
    } else {
      LOG_WARNING(
          "Can not set fitting parameter without setting fit algorithm");
      return;
    }
  }

  LOG_WARNING(_name, " does not need \"double\" parameter \"", n, "\"");
}

void SegAlg::set_parameter(string n, int v) {
  if (n == "numseg") {
    _number_of_segment = v;
    return;
  }

  LOG_WARNING(_name, " does not need \"integer\" parameter \"", n, "\"");
}

void SegAlg::set_parameter(string n, string v) {
  LOG_WARNING(_name, " does not need \"string\" parameter \"", n, "\"");
}

/// get result
shared_ptr<vector<Segment> > SegAlg::get_result() { return _result; }
