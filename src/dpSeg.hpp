/**
 * @file   dpSeg.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Fri Apr 10 11:07:36 2015
 *
 * @brief  segmentation using dynamic algorithm
 *
 *
 */

#ifndef DPSEG_H
#define DPSEG_H

#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

#include "SegAlg.hpp"

class DpSeg : public SegAlg {
public:
  DpSeg();
  virtual ~DpSeg() {};

  void run();

  void dump();

  void set_string(shared_ptr<vector<double> > &pd);

  void set_parameter(string n, double v);

protected:
  /// switch model penalty
  double _inputedSwitchModelPenalty;
  double _scaledSwitchModelPenalty;
  /// coefficient matrix (storing \f$a,b,c\f$)
  vector<double> _coefMatrix;
  /// storing the smallest penalty until every data point
  vector<double> _forwardValue;
  vector<int> _segBeginIndex;
  /// Intermedian value to speed up computing, storing the penalty of
  /// every possible choice of head index in inner loop of the algorithm
  vector<double> _penalty;
  /// Intermedian value to speed up computing, storing the storing
  ///  \f$a,b,c\f$ of every possible segment in inner loop of the algorithm
  vector<double> _tmpCoefMatrix;
  /// store the fitting order
  vector<FITORDER> _fitOrder;
  vector<FITORDER> _tmpOrder; // for computation
  /// used in setting string
  vector<double> _tmpString;
  /// set the scale of the series
  double _scale;

  shared_ptr<vector<double> > _shrS;
};

#endif /* DPSEG_H */
