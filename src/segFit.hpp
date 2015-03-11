/**
 * @file   segFit.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Sun Dec 22 00:19:27 2013
 * 
 * @brief  Declare the SegFit class
 * 
 * 
 */

#ifndef _SEGFIT_H_
#define _SEGFIT_H_

#include <vector>
using std::vector;

#include "./vpabcFit.hpp"

/**
 * Used to store the information * of every segments.
 * "\f$a,b,c\f$" are parameters of \f$y=ax^b+c\f$. 
 * "headIndex" and "tailIndex" are the index of head
 * and tail of the segment resp. (both inclusive)
 *
 */

enum FITORDER {ASCEND, DESCEND};

struct Segment
{
  int headIndex;
  int tailIndex;
  double a;
  double b;
  double c;
  FITORDER order; // the fitting order, i.e x = "1->T" or "T->1"
};


/**
 * @brief Main class implementing the interface of 
 * SegFit algorithm. It holds the memory arrays 
 * SegFit::_x0, SegFit::_lb, SegFit::_ub, SegFit::_btype
 * (due to the design of class Program)
 * 
 */

class SegFit
{
public:
  
  
  SegFit(double smp, int btype, double lb, double ub, int m, int maxiter, double factr, double pgtol);
  virtual ~SegFit();

  /** 
   * Set the string to be "segfitted"
   * 
   * @param pd point to the string
   * @param l the length of the string
   */
  void setString(double* pd, int l);

  void setSwitchModelPenalty(double p);

  /** 
   * Run the algorithm
   * 
   */
  void run();

  /**
   * Get the result of the algorithm
   * 
   */
  const vector< Segment >* getResult() { return _result; }

  void setScale(double scale);

private:

  /// A pointer of class VpabcFit
  VpabcFit* _vpabcFit_asc;
  VpabcFit* _vpabcFit_des;

  /// The boundary type of every parameter (currently just for \f$b\f$)
  int* _btype;
  /// The lower bound of every parameter  (currently just for \f$b\f$)
  double* _lb;
  /// The upper bound of every parameter  (currently just for \f$b\f$)
  double* _ub;
  /// The pointer storing every parameter  (currently just for \f$b\f$)
  /// Use the name "x0" because if one can set the initial point of lbfgsb
  /// algorithm by setting its value; Actually, by design, this parameter
  /// should not be in this class, but due to the design of class Program,
  /// this may be the "most clear" way to store them here.
  double* _x0;

  /// The length of the string
  int _length;
  /// switch model penalty
  double _inputedSwitchModelPenalty;
  double _scaledSwitchModelPenalty;
  /// coefficient matrix (storing \f$a,b,c\f$) 
  double * _coefMatrix;
  /// storing the smallest penalty until every data point
  double * _forwardValue;
  int * _segBeginIndex;
  /// Intermedian value to speed up computing, storing the penalty of
  /// every possible choice of head index in inner loop of the algorithm
  double * _penalty;
  /// Intermedian value to speed up computing, storing the storing
  ///  \f$a,b,c\f$ of every possible segment in inner loop of the algorithm
  double * _tmpCoefMatrix;
  /// store the fitting order
  FITORDER * _fitOrder;
  FITORDER * _tmpOrder; // for computation
  /// used in setting string
  double * _tmpString;
  /// store the result of the algorithm
  vector< Segment >* _result;
  /// set the scale of the series
  double _scale;
  
};

#endif /* _SEGFIT_H_ */
