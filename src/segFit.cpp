/**
 * @file   plysOptimizer.cpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Mon Oct 14 21:16:04 2013
 * 
 * @brief  Class SegFit holds the memory of _x0, _lb, _ub, _btype;
 * 
 * 
 */


// c++ standard headers
#include <iostream>
#include <algorithm>
#include <cmath>

using std::reverse;

#include "segFit.hpp"


SegFit::SegFit(double smp, int btype, double lb, double ub, int m, int maxiter, double factr, double pgtol)
{
  _scale = 100;
  _inputedSwitchModelPenalty = smp;
  
  _x0 = new double[1]; _x0[0] = 0;
  _lb = new double[1]; _lb[0] = lb;
  _ub = new double[1]; _ub[0] = ub;
  _btype = new int[1]; _btype[0] = btype;

  _vpabcFit_asc = new VpabcFit( _x0, _lb, _ub, _btype, m, maxiter, factr, pgtol);
  _vpabcFit_des= new VpabcFit( _x0, _lb, _ub, _btype, m, maxiter, factr, pgtol);

  /* initialize other string concerned values */
  _length = 0;
  _tmpCoefMatrix = NULL;
  _coefMatrix = NULL;
  _forwardValue = NULL;
  _penalty = NULL;
  _segBeginIndex = NULL;
  _fitOrder = NULL;
  _tmpOrder = NULL;
  _tmpString = NULL;
  
  _result = new vector< Segment >;
};

SegFit::~SegFit() {

  if( _x0 != NULL )
    {
      delete[] _x0;
      delete[] _btype;
      delete[] _ub;
      delete[] _lb;
      delete _vpabcFit_asc;
      delete _vpabcFit_des;

      delete[] _tmpCoefMatrix;
      delete[] _coefMatrix;
      delete[] _forwardValue;
      delete[] _penalty;
      delete[] _segBeginIndex;
      delete[] _fitOrder;
      delete[] _tmpOrder;
      delete[] _tmpString;
    }

  delete _result;
}

void SegFit::setSwitchModelPenalty(double p)
{
  _inputedSwitchModelPenalty = p;
}

void SegFit::setScale(double s)
{
  _scale = s;

  _vpabcFit_asc->setScale(s);
  _vpabcFit_des->setScale(s);
}


void SegFit::setString(double* pd, int l) {

  if (_length == 0) /* first string */
    {
      _length = l;
  
      _tmpCoefMatrix = new double [3*l];
      _coefMatrix = new double [3*l];
      _forwardValue = new double [l];
      _segBeginIndex = new int [l];
      _penalty = new double  [l];  /* used in inner loop for computation */
      _fitOrder = new FITORDER [l];
      _tmpOrder = new FITORDER [l];
      _tmpString = new double [l];

    }
  else if (_length == l) // new string with same length, does not need to manage memory
    {

    }
  else // new string with different length
    {
      _length = l;

      delete[] _tmpCoefMatrix;
      delete[] _coefMatrix;
      delete[] _forwardValue;
      delete[] _segBeginIndex;
      delete[] _penalty;
      
      _tmpCoefMatrix = new double [3*l];
      _coefMatrix = new double [3*l];
      _forwardValue = new double [l];
      _segBeginIndex = new int [l];
      _penalty = new double  [l];  /* used in inner loop for computation */
      _fitOrder = new FITORDER [l];
      _tmpOrder = new FITORDER [l];
      _tmpString = new double [l];

    }

  // ascending order 
  _vpabcFit_asc->setString(pd, l);

  // descending order
  for ( int i = 0; i < l; i++ )
    {
      _tmpString[l-i-1] = pd[i];
    }
  _vpabcFit_des->setString(_tmpString, l);
  
}

inline void min(double* d, int ei, double& minv, int& mini)
{
  minv = d[0];
  mini = 0;
  for (int i = 3; i < ei; i++)
    {
      if ( d[i] < minv )
	{
	  minv = d[i];
	  mini = i;
	}
    }
}

void SegFit::run() {

  int tmpIdx = 0;
  double tmpPenalty = 0;

  _scaledSwitchModelPenalty = _inputedSwitchModelPenalty/10000.0*_scale*_scale*_length;

  for ( int mainIdx = 3; mainIdx <= _length; mainIdx ++ )
    {
      _vpabcFit_asc->setIndex( 0, mainIdx ); // set object string to be [0, mainIdx)
      _vpabcFit_asc->runSolver();

      _vpabcFit_des->setIndex( _length-mainIdx, _length );
      _vpabcFit_des->runSolver();
      
      tmpPenalty = _vpabcFit_asc->getObjective();

      if ( tmpPenalty <= (_vpabcFit_des->getObjective()) )
	{ // perfer ascending order
	  _penalty[0] = _vpabcFit_asc->getObjective();
	  _vpabcFit_asc->getParameter(_tmpCoefMatrix[0], _tmpCoefMatrix[1], _tmpCoefMatrix[2]);
	  _tmpOrder[0] = ASCEND;
	}
      else
	{ // perfer descending order
	  _penalty[0] = _vpabcFit_des->getObjective();
	  _vpabcFit_des->getParameter(_tmpCoefMatrix[0], _tmpCoefMatrix[1], _tmpCoefMatrix[2]);
	  _tmpOrder[0] = DESCEND;
	}

      
      /* then use other head index in [3, mainIdx-2) */
      for ( int subIdx = 3; subIdx < mainIdx-2; subIdx++ )
	{
	  tmpIdx = 3*subIdx;

	  _vpabcFit_asc->setIndex( subIdx, mainIdx ); // set object string to be [subIdx, mainIdx)
	  _vpabcFit_asc->runSolver();

	  _vpabcFit_des->setIndex( _length-mainIdx, _length-subIdx ); // set object string to be [subIdx, mainIdx)
	  _vpabcFit_des->runSolver();
	  
	  tmpPenalty = _vpabcFit_asc->getObjective();

	  if ( tmpPenalty <= (_vpabcFit_des->getObjective()) )
	    { // perfer ascending order
	      _penalty[subIdx] = _forwardValue[subIdx-1] + _vpabcFit_asc->getObjective() + _scaledSwitchModelPenalty;
	      _vpabcFit_asc->getParameter(_tmpCoefMatrix[tmpIdx], _tmpCoefMatrix[tmpIdx+1], _tmpCoefMatrix[tmpIdx+2]);
	      _tmpOrder[subIdx] = ASCEND;
	    }
	  else
	    { // perfer descending order
	      _penalty[subIdx] = _forwardValue[subIdx-1] + _vpabcFit_des->getObjective() + _scaledSwitchModelPenalty;
	      _vpabcFit_des->getParameter(_tmpCoefMatrix[tmpIdx], _tmpCoefMatrix[tmpIdx+1], _tmpCoefMatrix[tmpIdx+2]);
	      _tmpOrder[subIdx] = DESCEND;
	    }

	}

      min( _penalty, mainIdx-2, _forwardValue[mainIdx-1], _segBeginIndex[mainIdx-1]);

      _fitOrder[mainIdx-1] = _tmpOrder[_segBeginIndex[mainIdx-1]];
      
      _coefMatrix[3*mainIdx-3] = _tmpCoefMatrix[3*_segBeginIndex[mainIdx-1]];
      _coefMatrix[3*mainIdx-2] = _tmpCoefMatrix[3*_segBeginIndex[mainIdx-1]+1];
      _coefMatrix[3*mainIdx-1] = _tmpCoefMatrix[3*_segBeginIndex[mainIdx-1]+2];
      
    }

  // update result
  _result->clear();

  // Segment tmpSeg;
  // int i = _length - 1;
  
  // while ( i > 0 )
  //   {
  //     tmpSeg.headIndex = _segBeginIndex[i];
  //     tmpSeg.tailIndex = i;
  //     tmpSeg.a = _coefMatrix[3*i-3];
  //     tmpSeg.b = _coefMatrix[3*i-2];
  //     tmpSeg.c = _coefMatrix[3*i-1];
  //     tmpSeg.order = _fitOrder[i];

  //     i = _segBeginIndex[i]-1;

  //     _result->push_back(tmpSeg);
  //   }

  // reverse(_result->begin(), _result->end());

  Segment tmpSeg;
  int i = _length - 1;
  
  while ( i > 0 )
    {
      tmpSeg.headIndex = _segBeginIndex[i];
      tmpSeg.tailIndex = i;
      tmpSeg.a = _coefMatrix[3*i];
      tmpSeg.b = _coefMatrix[3*i+1];
      tmpSeg.c = _coefMatrix[3*i+2];
      tmpSeg.order = _fitOrder[i];

      i = _segBeginIndex[i]-1;

      _result->push_back(tmpSeg);
    }

  reverse(_result->begin(), _result->end());

}
