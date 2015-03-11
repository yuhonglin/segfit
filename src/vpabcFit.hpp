/**
 * @file   vpabcFit.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Wed Dec 18 15:37:41 2013
 * 
 * @brief  Header file of class "VpabcFit"
 *         "vpabc" have two meanings:
 *            1) use variable projection
 *            2) fitting \f$y=aT^b+c\f$
 * 
 * 
 */


#ifndef _VPABCPROGRAM_H_
#define _VPABCPROGRAM_H_

#include "program.h"

/**
 * This class fit the curve \f$y=aT^b+c\f$.
 * \f$T\f$ has values \f$1,2,3...\f$.
 * User can fit any continuous sub string
 * by setting the start index and end index
 * (without nearly any overhead).
 */

class VpabcFit : public Program
{
public:
  VpabcFit(double* x, double* lb, double* ub, int* btype,
	   int m, int maxiter,
	   double factr, double pgtol);
  virtual ~VpabcFit();
  
  void computeGradient (int n, double* x, double* g);
  double computeObjective (int n, double* x);

  /** 
   * Set the string to be fitted. *MUST BE CALLED BEFORE SOLVING*
   * 
   * @param pd 
   * @param l 
   */
  void setString(double* pd, int l);

  /** 
   * Set the substring to fit (denoted by start and end index)
   * 
   * @param si start index
   * @param ei end index
   */
  void setIndex(int si, int ei);

  double getObjective() {
    return f;
  }

  void getParameter(double &a, double &b, double &c) {
    computeAC(); // this line may not be needed
    a = _a;
    b = _b;
    c = _c;
  }

  void setScale(double s);
  /* display */
  void dump();

private:
  /// the string
  double* _S;
  /// the length of the string
  int _length;
  /// Suffer values to speed up, storing \f$\log(T), T=1,2,3...\f$
  double* _logT;  /* log(T) */
  /// Storing \f$T^b, T=1,2,3...\f$
  double* _Tb;  
  /// difference between fitted value and VpabcFit::_S
  double* _diff;  

  /// the scale of string
  double _scale;
  
  /// \f${\bf [}\_startindx, \_endIndex{\bf)}\f$
  int _startIdx;
  /// \f${\bf [}\_startindx, \_endIndex{\bf)}\f$
  int _endIdx;

  /// the parameters of the curve
  double _a, _b, _c;

  /// intermedian value (computing pseudoinverse) \f$\Phi^T\Phi\f$
  double _PhiTPhi[4];
  /// intermedian value (computing pseudoinverse) \f$\Phi^T*S\f$
  double _PhiTS[2];
  /// intermedian value (computing pseudoinverse)
  double _bbdd, _adbc;

  /// Compute VpabcFit::_a and VpabcFit::_c from VpabcFit::_b
  void computeAC();
  /// Set the initial point with heuristic
  void setInitPoint();

};

#endif /* _VPABCPROGRAM_H_ */
