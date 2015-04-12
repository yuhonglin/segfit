#include "SegAlgFactory.hpp"
#include "dpSeg.hpp"
#include "logger.hpp"
#include "topDownSeg.hpp"

#include <iostream>

unique_ptr<SegAlg> SegAlgFactory::make(string n) {

  if (n == "dp") {
    return std::move(unique_ptr<SegAlg>(new DpSeg()));
  }

  if (n == "topdown") {
    return std::move(unique_ptr<SegAlg>(new TopDownSeg()));
  }

  LOG_WARNING("unkown segmentation method : \"", n, "\", use dynamic programming as default");

  return std::move(unique_ptr<SegAlg>(new DpSeg()));
}
