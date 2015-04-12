#include <sstream>
#include <cmath>

#include "logger.hpp"
#include "ablineFit.hpp"

AblineFit::AblineFit() : FitAlg(), _a(_segment.a), _b(_segment.b) {
  _name = "AblineFit";
  _numParameter = 2;
}

AblineFit::~AblineFit() {}

double AblineFit::fit() {
  /// compute the mean
  double meanX = (1.0 + _length) / 2;
  double meanY = 0;
  double inverseLength = 1.0 / _length;
  for (int i = 0; i < _length; i++) {
    meanY += (*_shrS)[_startIdx + i] * inverseLength; /// avoid overflow
  }

  /// compute a,b
  _a = 0;
  double denominator = 0.0;
  for (int i = 1; i <= _length; i++) {
    denominator += pow(i - meanX, 2);
  }
  double inverseDenominator = 1.0 / denominator;
  for (int i = 0; i < _length; i++) {
    _a += ((i + 1 - meanX) * ((*_shrS)[_startIdx + i] - meanY) *
           inverseDenominator);
  }

  _b = meanY - _a * meanX;

  /// compute loss
  double ret = 0.0;
  for (int i = 1; i <= _length; i++) {
    ret += pow(_a * i + _b - (*_shrS)[_startIdx + i - 1], 2);
  }

  _segment.loss = 0.5*ret;

  return 0.5*ret;
}

void AblineFit::dump() {
  std::stringstream ss;
  for (int i = 0; i < _length; i++) {
    ss << (*_shrS)[_startIdx + i] << ' ';
  }
  LOG_INFO(ss.str());

  ss.str("");
  ss << "_length: " << _length << "\ta: " << _a << "\tb: " << _b << std::endl;

  LOG_INFO(ss.str());
}
