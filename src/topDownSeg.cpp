#include <algorithm>
#include <sstream>
#include <limits>
#include <vector>
#include <queue>

#include "topDownSeg.hpp"
#include "logger.hpp"

TopDownSeg::TopDownSeg() : SegAlg(), _threshold(.01) { _name = "topdown"; }

TopDownSeg::~TopDownSeg() {}

bool operator<(const SegWithCutLoss &lhs, const SegWithCutLoss &rhs) {
  return (lhs.seg.loss - lhs.minLoss) < (rhs.seg.loss - rhs.minLoss);
}

void TopDownSeg::setBestCut(SegWithCutLoss &s, int isLeft) {
  double minLoss = std::numeric_limits<double>::max();
  double bestCut = 0;
  Segment leftSeg, rightSeg;
  for (int i = s.seg.headIndex + 3; i <= s.seg.tailIndex - 2; i++) {

    if (isLeft == 0) {
      // initial need to do both
      _fitAlg->set_index(s.seg.headIndex, i);
      _fitAlg->fit();
      leftSeg = _fitAlg->get_segment();
      _boundary_segment[pair<int, int>(s.seg.headIndex, i)] = leftSeg;
      _fitAlg->set_index(i, s.seg.tailIndex);
      _fitAlg->fit();
      rightSeg = _fitAlg->get_segment();
      _boundary_segment[pair<int, int>(i, s.seg.tailIndex + 1)] = rightSeg;
    } else if (isLeft == 1) {
      // no need to fit left
      leftSeg = _boundary_segment[pair<int, int>(s.seg.headIndex, i)];
      // fit the right
      _fitAlg->set_index(i, s.seg.tailIndex);
      _fitAlg->fit();
      rightSeg = _fitAlg->get_segment();
      // store the result
      _boundary_segment[pair<int, int>(i, s.seg.tailIndex + 1)] = rightSeg;
    } else if (isLeft == 2) {
      // fit the left
      _fitAlg->set_index(s.seg.headIndex, i);
      _fitAlg->fit();
      leftSeg = _fitAlg->get_segment();
      _boundary_segment[pair<int, int>(s.seg.headIndex, i)] = leftSeg;
      // no need to fit right
      rightSeg = _boundary_segment[pair<int, int>(i, s.seg.tailIndex + 1)];
    }

    if (leftSeg.loss + rightSeg.loss < minLoss) {
      minLoss = leftSeg.loss + rightSeg.loss;
      bestCut = i;
    }
  }

  s.minLoss = minLoss;
  s.bestCut = bestCut;
}

void TopDownSeg::run() {
  SegWithCutLoss currentSeg, leftSeg, rightSeg;
  std::priority_queue<SegWithCutLoss> segQueue;
  // use leftSeg to init the queue
  _fitAlg->set_index(0, _shrS->size());
  _fitAlg->fit();
  currentSeg.seg = _fitAlg->get_segment();
  setBestCut(currentSeg, 0);
  segQueue.push(currentSeg);

  // begin cutting
  double totalLoss = currentSeg.seg.loss;

  while (totalLoss > _threshold &&
         (segQueue.top().seg.tailIndex - segQueue.top().seg.headIndex) >=
             2 * _fitAlg->get_numParam()) {

    // if the number of segments is met
    if (_number_of_segment - segQueue.size() == 0) {
      break;
    }
    
    // main process
    currentSeg = segQueue.top();
    segQueue.pop();
    // fit the left sub segment
    leftSeg.seg = _boundary_segment
        [pair<int, int>(currentSeg.seg.headIndex, currentSeg.bestCut)];
    // update its best cut and push to queue
    setBestCut(leftSeg, 1);
    segQueue.push(leftSeg);

    // fit the right sub segment
    rightSeg.seg = _boundary_segment
        [pair<int, int>(currentSeg.bestCut, currentSeg.seg.tailIndex + 1)];
    // update its best cut and push to queue
    setBestCut(rightSeg, 2);
    segQueue.push(rightSeg);

    totalLoss = totalLoss - currentSeg.seg.loss + currentSeg.minLoss;
  }

  // generate result
  _result->clear();
  _result->reserve(segQueue.size());

  while (!segQueue.empty()) {
    _result->push_back(segQueue.top().seg);
    _result->back().tailIndex--;
    segQueue.pop();
  }

  std::sort(_result->begin(), _result->end(),
            [](const Segment & a, const Segment & b)
                ->bool { return a.headIndex < b.headIndex; });

  // clear computation record
  _boundary_segment.clear();
}

void TopDownSeg::set_string(shared_ptr<vector<double> > &pd) {

  if (_fitAlg != std::nullptr_t()) {
    _shrS = pd;
    _fitAlg->set_string(pd);

  } else {
    LOG_WARNING("not set fitting algorithm yet, can not set string.");
  }
}

void TopDownSeg::dump() {
  LOG_INFO("-*- [Infomation of ", _name, "]");
  LOG_INFO("fitting method : ", _fitAlg->get_name());
  LOG_INFO("threshold=", _threshold);
  
  std::stringstream ss;
  ss << "Scaled string :";
  for (auto s : (*_shrS)) {
    ss << " " << s;
  }
  LOG_INFO(ss.str());

  int i = 0;
  for (auto s : (*_result)) {
    ss.str("");
    ss << "Segment-" << i << " : a=" << s.a << " b=" << s.b << " c=" << s.c
       << " loss: " << s.loss << " startIdx=" << s.headIndex
       << " endIdx=" << s.tailIndex;
    LOG_INFO(ss.str());
    i++;
  }
  LOG_INFO("-*-");
}
