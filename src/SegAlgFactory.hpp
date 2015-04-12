#ifndef SEGALGFACTORY_H
#define SEGALGFACTORY_H

#include <memory>

#include "SegAlg.hpp"

using std::unique_ptr;

class SegAlgFactory {
public:
  SegAlgFactory() {};
  virtual ~SegAlgFactory() {};

  static unique_ptr<SegAlg> make(string type);
};

#endif /* SEGALGFACTORY_H */
