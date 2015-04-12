/**
 * @file   FitAlg.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Tue Apr  7 11:26:04 2015
 *
 * @brief  Base class of fitting method
 *
 *
 */

#ifndef FITALG_H
#define FITALG_H

#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using std::shared_ptr;

/**
 * Fitting order
 * ASCEND  : T = 1,2,3...,n
 * DESCEND : T = n,n-1...,1
 *
 */

enum FITORDER {
  ASCEND,
  DESCEND
};

/**
 * Struct representing each segment
 *
 */

struct Segment {
  int headIndex;
  int tailIndex;
  double a;
  double b;
  double c;
  double loss;
  FITORDER order;
};

/**
 * The fitting base class
 *
 */

class FitAlg {
public:
  FitAlg();
  virtual ~FitAlg() {};

  /// set fitting index
  void set_index(int si, int ei);

  /// fit the curve on [startIdx, endIdx]
  virtual double fit() = 0;

  /// get parameter
  virtual Segment get_segment();

  /// set parameter
  virtual void set_parameter(string n, double v);
  virtual void set_parameter(string n, string v);

  /// set string and scale
  virtual void set_string(shared_ptr<vector<double> > &vpd);

  /// dump information
  virtual void dump();

  /// get number of parameter
  int get_numParam();

protected:
  /// the string, use a raw pointer aside the smart
  /// pointer due to speed concerns
  shared_ptr<vector<double> > _shrS;

  /// the length of the string
  int _length;
  /// \f${\bf [}\_startindx, \_endIndex{\bf)}\f$
  int _startIdx;
  /// \f${\bf [}\_startindx, \_endIndex{\bf)}\f$
  int _endIdx;
  /// parameter
  Segment _segment;

  /// set initial value
  virtual void init();

  /// algorithm name
  string _name;

  /// number of parameter in the curves
  int _numParameter;
};

#endif /* FITALG_H */
