/**
 * @file   ablineFit.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Thu Apr  9 13:57:44 2015
 *
 * @brief  fit ```y=ax+b```
 *
 *
 */

#ifndef ABLINEFIT_H
#define ABLINEFIT_H

#include "FitAlg.hpp"

class AblineFit : public FitAlg {
public:
  AblineFit();
  virtual ~AblineFit();

  double fit();

  void dump();

private:
  double &_a, &_b;
};

#endif /* ABLINEFIT_H */
