/**
 * @file   vpabcFit.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Wed Apr  8 20:51:44 2015
 *
 * @brief  fit y=a*x^b+c
 *
 *
 */

#ifndef VPABCFIT_H
#define VPABCFIT_H

#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;

#include "program.h"
#include "FitAlg.hpp"

class VpabcFit : public Program, public FitAlg {
public:
  VpabcFit();
  virtual ~VpabcFit();

  /// set the string
  virtual void set_string(shared_ptr<vector<double> > &S);

  void set_scale(double s);

  double fit();

  Segment get_segment();

  void dump();

  void set_parameter(string n, double v);

protected:
  double get_objective() { return f; }

  void computeGradient(int n, double *x, double *g);
  double computeObjective(int n, double *x);

  void compute_ac();

  /// Set the initial point with heuristic
  void init();

private:
  /// Suffer values to speed up, storing \f$\log(T), T=1,2,3...\f$
  vector<double> _logT; /* log(T) */
  /// Storing \f$T^b, T=1,2,3...\f$
  vector<double> _Tb;
  /// difference between fitted value and VpabcFit::_S
  vector<double> _diff;

  /// the parameters of the curve
  double &_a, &_b, &_c;

  /// intermedian value (computing pseudoinverse) \f$\Phi^T\Phi\f$
  double _PhiTPhi[4];
  /// intermedian value (computing pseudoinverse) \f$\Phi^T*S\f$
  double _PhiTS[2];
  /// intermedian value (computing pseudoinverse)
  double _bbdd, _adbc;
};

#endif /* VPABCFIT_H */
