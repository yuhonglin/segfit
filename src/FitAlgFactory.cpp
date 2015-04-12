#include "logger.hpp"
#include "ablineFit.hpp"
#include "vpabcFit.hpp"
#include "constFit.hpp"
#include "FitAlgFactory.hpp"

FitAlgFactory::FitAlgFactory() : _numInstance(0) {}

unique_ptr<FitAlg> FitAlgFactory::make(string n) {
  if (n == "abline") {
    return std::move(unique_ptr<FitAlg>(new AblineFit()));
  }

  if (n == "powerabc") {
    return std::move(unique_ptr<FitAlg>(new VpabcFit()));
  }

  if (n == "const") {
    return std::move(unique_ptr<FitAlg>(new ConstFit()));
  }

  LOG_WARNING("unknown fitting method : \"", n, "\", use abline as default");

  return std::move(unique_ptr<FitAlg>(new AblineFit()));
}
