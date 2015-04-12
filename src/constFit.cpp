#include <sstream>
#include <cmath>

#include "logger.hpp"
#include "constFit.hpp"

ConstFit::ConstFit() : FitAlg(), _a(_segment.a) {
  _name = "constFit";
  _numParameter = 1;
}

ConstFit::~ConstFit() {}

double ConstFit::fit() {
  _a = 0;
  double inverseLength = 1.0 / _length;
  for (int i = 0; i < _length; i++) {
    _a += (*_shrS)[_startIdx + i] * inverseLength; /// avoid overflow
  }

  double ret = 0.0;
  for (int i = 1; i <= _length; i++) {
    ret += pow(_a - (*_shrS)[_startIdx + i - 1], 2);
  }

  _segment.loss = ret;

  return ret;
}

void ConstFit::dump() {
  std::stringstream ss;
  for (int i = 0; i < _length; i++) {
    ss << (*_shrS)[_startIdx + i] << ' ';
  }
  LOG_INFO(ss.str());

  ss.str("");
  ss << "_length: " << _length << "\ta: " << _a << std::endl;

  LOG_INFO(ss.str());
}
