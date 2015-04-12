/**
 * @file   FitAlgFactory.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Fri Apr 10 10:03:58 2015
 *
 * @brief  The fitting algorithm factory
 *
 *
 */

#include <memory>
#include <string>

using std::unique_ptr;
using std::string;

#include <FitAlg.hpp>

class FitAlgFactory {
public:
  FitAlgFactory();
  virtual ~FitAlgFactory() {};

  unique_ptr<FitAlg> make(string n);

  void dump();

private:
  int _numInstance;
};
