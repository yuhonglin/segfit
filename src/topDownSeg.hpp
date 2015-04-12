/**
 * @file   topDownSeg.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Sat Apr 11 09:09:34 2015
 *
 * @brief  the top-down segmentation method
 *
 *
 */

#include <memory>
#include <vector>
#include <map>
#include <utility>

using std::unique_ptr;
using std::vector;
using std::map;
using std::pair;

#include "SegAlg.hpp"

struct SegWithCutLoss {
  Segment seg;
  double minLoss;
  int bestCut;
};

class TopDownSeg : public SegAlg {
public:
  TopDownSeg();
  virtual ~TopDownSeg();

  void run();

  void dump();

  void set_string(shared_ptr<vector<double> > &pd);

private:
  double _threshold;

  /// compute minLoss and bestCut based on seg
  void setBestCut(SegWithCutLoss &s, int isLeft);

  /// record the fitting result to avoid refit
  map<pair<int, int>, Segment> _boundary_segment;
};
