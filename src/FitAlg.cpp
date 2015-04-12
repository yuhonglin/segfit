#include <limits>
#include <stdexcept>

#include "FitAlg.hpp"
#include "logger.hpp"

FitAlg::FitAlg() {
  _startIdx = 0;
  _endIdx = 0;
  _length = 0;

  _scale = 100.0;

  _segment.headIndex = std::numeric_limits<int>::quiet_NaN();
  _segment.tailIndex = std::numeric_limits<int>::quiet_NaN();
  _segment.a = std::numeric_limits<double>::quiet_NaN();
  _segment.b = std::numeric_limits<double>::quiet_NaN();
  _segment.c = std::numeric_limits<double>::quiet_NaN();
  _segment.order = ASCEND;
}

Segment FitAlg::get_segment() { return _segment; }

void FitAlg::set_index(int si, int ei) {

  _startIdx = si;
  _endIdx = ei;

  _segment.headIndex = _startIdx;
  _segment.tailIndex = _endIdx;

  _length = _endIdx - _startIdx;

  /// may initialize intial variable
  init();
}

/// default is empty
void FitAlg::init() {}

/// parameter interface
void FitAlg::set_parameter(string n, double v) {
  LOG_WARNING(_name, " does not need \"double\" parameter \"", n, "\"");
}

void FitAlg::set_parameter(string n, string v) {
  LOG_WARNING(_name, " does not need \"string\" parameter \"", n, "\"");
}

/// set string
void FitAlg::set_string(shared_ptr<vector<double> > &vpd) {
  _shrS = vpd;

  double maxS = 0;
  double rate = 0;

  // get max
  for (unsigned int i = 0; i < _shrS->size(); i++) {
    if ((*_shrS)[i] > maxS) {
      maxS = (*_shrS)[i];
    }
  }
  // compute rate to save time
  if (maxS != 0) {
    rate = _scale / maxS;
  }
  // scale
  for (unsigned int i = 0; i < _shrS->size(); i++) {
    (*_shrS)[i] *= rate;
  }
}

void FitAlg::dump() {
  std::stringstream ss;
  for (int i = 0; i < _length; i++) {
    ss << (*_shrS)[_startIdx + i] << ' ';
  }
  LOG_INFO(ss.str());

  ss.str("");
  ss << "a:" << _segment.a << " b:" << _segment.b << " c:" << _segment.c;

  LOG_INFO(ss.str());
}

int FitAlg::get_numParam() { return _numParameter; }

string FitAlg::get_name() { return _name; }
