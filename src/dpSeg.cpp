#include <algorithm>
#include <sstream>

#include "dpSeg.hpp"
#include "logger.hpp"

inline void min(vector<double> &d, int ei, double &minv, int &mini) {
  minv = d[0];
  mini = 0;
  for (int i = 3; i < ei; i++) {
    if (d[i] < minv) {
      minv = d[i];
      mini = i;
    }
  }
}

DpSeg::DpSeg() {
  _scale = 100.0;
  _inputedSwitchModelPenalty = 2.3;
};

void DpSeg::run() {

  int tmpIdx = 0;
  Segment tmpSegment;

  vector<double> tmpFitLoss(_shrS->size());
  vector<double> fitLoss(_shrS->size());

  _scaledSwitchModelPenalty =
      _inputedSwitchModelPenalty / 10000.0 * _scale * _scale * _shrS->size();

  for (unsigned int mainIdx = 3; mainIdx <= _shrS->size(); mainIdx++) {
    _fitAlg->set_index(0, mainIdx); // set object string to be [0, mainIdx)

    _penalty[0] = _fitAlg->fit();
    tmpSegment = _fitAlg->get_segment();
    _tmpCoefMatrix[0] = tmpSegment.a;
    _tmpCoefMatrix[1] = tmpSegment.b;
    _tmpCoefMatrix[2] = tmpSegment.c;
    _tmpOrder[0] = ASCEND;

    tmpFitLoss[0] = tmpSegment.loss;

    /* then use other head index in [3, mainIdx-2) */
    for (unsigned int subIdx = 3; subIdx < mainIdx - 2; subIdx++) {
      tmpIdx = 3 * subIdx;

      _fitAlg->set_index(subIdx,
                         mainIdx); // set object string to be [subIdx, mainIdx)

      _penalty[subIdx] = _forwardValue[subIdx - 1] + _fitAlg->fit() +
                         _scaledSwitchModelPenalty;
      tmpSegment = _fitAlg->get_segment();
      _tmpCoefMatrix[tmpIdx] = tmpSegment.a;
      _tmpCoefMatrix[tmpIdx + 1] = tmpSegment.b;
      _tmpCoefMatrix[tmpIdx + 2] = tmpSegment.c;
      _tmpOrder[subIdx] = ASCEND;

      tmpFitLoss[subIdx] = tmpSegment.loss;
    }

    min(_penalty, mainIdx - 2, _forwardValue[mainIdx - 1],
        _segBeginIndex[mainIdx - 1]);

    _fitOrder[mainIdx - 1] = _tmpOrder[_segBeginIndex[mainIdx - 1]];

    _coefMatrix[3 * mainIdx - 3] =
        _tmpCoefMatrix[3 * _segBeginIndex[mainIdx - 1]];
    _coefMatrix[3 * mainIdx - 2] =
        _tmpCoefMatrix[3 * _segBeginIndex[mainIdx - 1] + 1];
    _coefMatrix[3 * mainIdx - 1] =
        _tmpCoefMatrix[3 * _segBeginIndex[mainIdx - 1] + 2];

    fitLoss[mainIdx - 1] = tmpFitLoss[_segBeginIndex[mainIdx - 1]];
  }

  // update result
  _result->clear();

  Segment tmpSeg;
  int i = _shrS->size() - 1;

  while (i > 0) {
    tmpSeg.headIndex = _segBeginIndex[i];
    tmpSeg.tailIndex = i;
    tmpSeg.a = _coefMatrix[3 * i];
    tmpSeg.b = _coefMatrix[3 * i + 1];
    tmpSeg.c = _coefMatrix[3 * i + 2];
    tmpSeg.order = _fitOrder[i];

    tmpSeg.loss = fitLoss[i];

    i = _segBeginIndex[i] - 1;

    _result->push_back(tmpSeg);
  }

  std::reverse(_result->begin(), _result->end());
}

void DpSeg::dump() {
  LOG_INFO("-*- [Infomation of dpSeg]");
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
       << " loss=" << s.loss << " startIdx=" << s.headIndex
       << " endIdx=" << s.tailIndex;
    LOG_INFO(ss.str());
    i++;
  }
  LOG_INFO("-*-");
}

void DpSeg::set_string(shared_ptr<vector<double> > &pd) {

  if (_fitAlg != std::nullptr_t()) {
    _shrS = pd;
    _fitAlg->set_string(pd);

    _tmpCoefMatrix.clear();
    _tmpCoefMatrix.resize(pd->size() * 3);

    _coefMatrix.clear();
    _coefMatrix.resize(pd->size() * 3);

    _forwardValue.clear();
    _forwardValue.resize(pd->size());

    _segBeginIndex.clear();
    _segBeginIndex.resize(pd->size());

    _penalty.clear();
    _penalty.resize(pd->size());

    _fitOrder.clear();
    _fitOrder.resize(pd->size());

    _tmpOrder.clear();
    _tmpOrder.resize(pd->size());

    _tmpString.clear();
    _tmpString.resize(pd->size());

  } else {
    LOG_WARNING("not set fitting algorithm yet, can not set string.");
  }
}

void DpSeg::set_parameter(string n, double v) {
  if (n == "eta") {
    _inputedSwitchModelPenalty = v;
    return;
  }
  LOG_WARNING("Unknown parameter \"", n, "\"");
}
