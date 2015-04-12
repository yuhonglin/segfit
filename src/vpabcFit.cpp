#include <sstream>
#include <cmath>

#include "logger.hpp"
#include "vpabcFit.hpp"

VpabcFit::VpabcFit()
    : Program(1), FitAlg(), _a(_segment.a), _b(*x), _c(_segment.c) {
  _length = 0;

  _scale = 100.0;

  _numParameter = 3;

  _startIdx = 0;
  _endIdx = 0;

  _name = "VpabcFit";

  ub[0] = 6;
  lb[0] = -6;
  btype[0] = 2;
}

VpabcFit::~VpabcFit() {}

double VpabcFit::computeObjective(int n, double *p) {

  //  _b = p[0];

  int i = 0; // loop value
  /* compute T.^b */
  for (i = 0; i < _length; i++) {
    _Tb[i] = pow(i + 1, _b);
  }

  /* compute _a and _c, must behind computing T.^b */
  compute_ac();

  /* compute diff */
  for (i = 0; i < _length; i++) {
    _diff[i] = _a * _Tb[i] + _c - (*_shrS)[_startIdx + i];
  }

  double ret = 0;
  for (i = 0; i < _length; i++) {
    ret += pow(_diff[i], 2);
  }

  return 0.5 * ret;
}

void VpabcFit::computeGradient(int n, double *p, double *df) {
  /* compute df */
  df[0] = 0;
  for (int i = 0; i < _length; i++) {
    df[0] += _a * _diff[i] * _Tb[i] * _logT[i];
  }
}

Segment VpabcFit::get_segment() {
  _segment.b = _b;
  return _segment;
}

void VpabcFit::compute_ac() {
  int i = 0;
  _PhiTS[0] = 0;
  _PhiTS[1] = 0;
  // first compute $\Phi*S$
  for (i = 0; i < _length; i++) {
    _PhiTS[0] += _Tb[i] * (*_shrS)[_startIdx + i];
    _PhiTS[1] += (*_shrS)[_startIdx + i];
  }

  // then compute $\Phi^T\Phi$
  _PhiTPhi[0] = 0;
  _PhiTPhi[1] = 0;
  for (i = 0; i < _length; i++) {
    _PhiTPhi[0] += pow(_Tb[i], 2);
    _PhiTPhi[1] += _Tb[i];
  }
  _PhiTPhi[2] = _PhiTPhi[1];
  _PhiTPhi[3] = _length;

  if (_PhiTPhi[1] * _PhiTPhi[1] == _PhiTPhi[0] * _PhiTPhi[3]) {
    // rank 1 (can not be rank 0, because _PhiTPhi[3]!=0)
    _bbdd = pow(_PhiTPhi[0] + _PhiTPhi[3], 2);
    _a = (_PhiTPhi[0] * _PhiTS[0] + _PhiTPhi[1] * _PhiTS[1]) / _bbdd;
    _c = (_PhiTPhi[1] * _PhiTS[0] + _PhiTPhi[3] * _PhiTS[1]) / _bbdd;
  } else {
    // full rank
    _adbc = _PhiTPhi[0] * _PhiTPhi[3] - _PhiTPhi[1] * _PhiTPhi[1];
    _a = (_PhiTPhi[3] * _PhiTS[0] - _PhiTPhi[1] * _PhiTS[1]) / _adbc;
    _c = (-_PhiTPhi[2] * _PhiTS[0] + _PhiTPhi[0] * _PhiTS[1]) / _adbc;
  }
}

void VpabcFit::init() {
  x[0] = 0;
  int n = 0;

  if ((*_shrS)[_startIdx] <= 0) { // can not log
    for (int i = 0; i < _length; i++) {
      if ((*_shrS)[_startIdx + i] > 0) {
        x[0] += ((*_shrS)[_startIdx + i] / (i + 1)); // version 1
        n++;
      }
    }
  } else {
    for (int i = 0; i < _length - 1; i++) {
      if ((*_shrS)[_startIdx + i + 1] > 0) {
        x[0] -= (log((*_shrS)[_startIdx] / (*_shrS)[_startIdx + i + 1]) /
                 _logT[i + 1]); // see comments of ```power2start``` function in
                                // curvefit toolbox (MATLAB)
        n++;
      }
    }
  }

  if (n > 2) {
    x[0] /= n;
  } else {
    x[0] = 0.5;
  }
}

void VpabcFit::set_scale(double s) {
  _scale = s;

  double rate = 0;

  if (_shrS->size() != 0) {
    double maxS = 0;
    for (unsigned int i = 0; i < _shrS->size(); i++) {
      if ((*_shrS)[i] > maxS) {
        maxS = (*_shrS)[i];
      }
    }

    if (maxS != 0) {
      rate = s / maxS;
    }

    for (unsigned int i = 0; i < _shrS->size(); i++) {
      (*_shrS)[i] *= rate;
    }
  }
}

void VpabcFit::set_string(shared_ptr<vector<double> > &vpd) {

  _shrS = vpd;

  int l = vpd->size();
  vector<double> &pd = (*vpd);

  double *_S = &((*_shrS)[0]);

  double maxS = 0;
  double rate = 0;

  for (int i = 0; i < l; i++) {
    if (pd[i] > maxS) {
      maxS = pd[i];
    }
  }
  if (maxS != 0) {
    rate = _scale / maxS;
  }

  _length = l;

  _diff.resize(l);
  _Tb.resize(l);
  _logT.resize(l);

  _startIdx = 0;
  _endIdx = l;

  for (int i = 0; i < l; i++) {
    _logT[i] = log(i + 1);
  }
  for (int i = 0; i < l; i++) {
    _S[i] = pd[i] * rate;
  }
}

void VpabcFit::dump() {
  std::stringstream ss;
  for (int i = 0; i < _length; i++) {
    ss << (*_shrS)[_startIdx + i] << ' ';
  }

  ss.str("");
  ss << "_length: " << _length << "\ta: " << _a << "\tb: " << _b
     << "\tc: " << _c << endl;

  LOG_INFO(ss.str());
}

double VpabcFit::fit() {
  this->runSolver();
  _segment.loss = f;
  return f;
}

void VpabcFit::set_parameter(string n, double v) {
  if (n == "scale") {
    set_scale(v);
    return;
  }

  if (n == "lowerboundofb") {
    lb[0] = v;
  }

  if (n == "upperboundofb") {
    ub[0] = v;
  }

  LOG_WARNING("Unknown parameter \"" + n + "\"");
}
