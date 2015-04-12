/**
 * @file   constFit.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Fri Apr 10 23:38:58 2015
 * 
 * @brief  fit ```y = a```
 * 
 * 
 */

#ifndef CONSTFIT_H
#define CONSTFIT_H

#include "FitAlg.hpp"

class ConstFit : public FitAlg
{
public:
  ConstFit();
  virtual ~ConstFit();

  double fit();

  void dump();

private:
  double &_a;
};


#endif /* CONSTFIT_H */
