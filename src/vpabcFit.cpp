#include <cmath>
#include <iostream>
using namespace std;


#include "vpabcFit.hpp"


VpabcFit::VpabcFit (double* x, double* lb, double* ub, int* btype,
			  int m = defaultm, int maxiter = defaultmaxiter,
			  double factr = defaultfactr, double pgtol = defaultpgtol)
  : Program ( 1, x, lb, ub, btype, m, maxiter, factr, pgtol)
{

  _length = 0;
  _S = NULL;
  _logT = NULL;
  _Tb = NULL;
  _diff = NULL;

  _startIdx = 0;
  _endIdx = 0;

  _a = 0;
  _b = 0;
  _c = 0;

  _scale = 100;
  
}


VpabcFit::~VpabcFit () {
  if (_length != 0)
    {
      delete[] _diff;
      delete[] _Tb;
      delete[] _logT;
      delete[] _S;      
    }
  _length = 0;
}

/**
 * Comments on the following two functions: ``computeObjective'' and ``computeGradient'':
 *
 * 1. computing objective and gradient have common steps like
 *    (1) compute _Tb (2) compute _diff
 * 2. so originally, objective and gradient functions should not be separated, otherwise
 * there will be a lot of recomputing. But this is the existing design of the interface.
 * 3. currently, it is assumed that the two functions are always computed one by one:
 * ``computeObjective'' first and ``computeGradient'' second. (see line 109,110 of file
 * program.cpp)
 * 4. So _Tb and _diff are updated in ``computeObjective'' and ``computeGradient'' directly
 * use it. By this the speed has been doubled.
 * 
 */

double VpabcFit::computeObjective(int n, double *p) {
  
  _b = p[0];
  
  int i = 0; // loop value
  /* compute T.^b */
  for( i = 0; i < _length; i++ )
    {
      _Tb[i] = pow( i+1, _b );

    }

  /* compute _a and _c, must behind computing T.^b */
  computeAC();
     
  /* compute diff */
  for( i = 0; i < _length; i++ )
    {
      _diff[i] = _a*_Tb[i] + _c - _S[_startIdx+i];
    }

  double ret = 0;
  for( i = 0; i < _length; i ++ )
    {
      ret += pow( _diff[i], 2 );
    }
  
  return 0.5*ret;
}

void VpabcFit::computeGradient(int n, double *p, double *df) {
  /* compute df */
  df[0] = 0;
  for(int i = 0; i < _length; i++ )
    {
      df[0] += _a * _diff[i] * _Tb[i] * _logT[i];
    }
}


void VpabcFit::setString(double* pd, int l) {

  double maxS = 0;
  double rate = 0;

  for ( int i = 0; i < l; i++ )
    {
      if ( pd[i] > maxS )
	{
	  maxS = pd[i];
	}
    }
  if ( maxS != 0 )
    {
      rate = _scale/maxS;
    }

  if (_length == 0) // new string
    {
      _length = l;
      _diff = new double[l];
      _Tb = new double[l];
      _logT = new double[l];
      _S = new double[l];
      
      _startIdx = 0;
      _endIdx = l;
      
      for( int i = 0; i < l; i++ )
	{
	  _logT[i] = log(i+1);
	  /* does not need to be initialized */
	  //_diff[i] = 0;
	  //_Tb[i] = 0;
	}
      for( int i = 0; i < l; i++ )
	{
	  _S[i] = pd[i]*rate;
	}
    }
  else if (_length == l) // new string with the same length
    {
      _startIdx = 0;
      _endIdx = l;
      for( int i = 0; i < l; i++ )
	{
	  _S[i] = pd[i]*rate;
	}
    }
  else  // new string with different length
    {
      delete[] _diff;
      delete[] _Tb;
      delete[] _logT;
      delete[] _S;
      
      _length = l;
      _diff = new double[l];
      _Tb = new double[l];
      _logT = new double[l];
      _S = new double[l];
      
      _startIdx = 0;
      _endIdx = l;
      
      for( int i = 0; i < l; i++ )
	{
	  _logT[i] = log(i+1);
	  /* does not need to be initialized */
	  //_diff[i] = 0;
	  //_Tb[i] = 0;
	}
      for( int i = 0; i < l; i++ )
	{
	  _S[i] = pd[i]*rate;
	}
    }
}
  
  
void VpabcFit::setIndex(int si, int ei) {
  
  _startIdx = si;
  _endIdx = ei;

  _length = _endIdx - _startIdx;
  
  setInitPoint();
  
}

void VpabcFit::computeAC()
{
  int i = 0;
  _PhiTS[0] = 0;
  _PhiTS[1] = 0;
  // first compute $\Phi*S$
  for( i = 0; i < _length; i++ )
    {
      _PhiTS[0] += _Tb[i]*_S[_startIdx+i];
      _PhiTS[1] += _S[_startIdx+i];
    }

  // then compute $\Phi^T\Phi$
  _PhiTPhi[0] = 0;
  _PhiTPhi[1] = 0;
  for( i = 0; i < _length; i++ )
    {
      _PhiTPhi[0] += pow(_Tb[i],2);
      _PhiTPhi[1] += _Tb[i];
    }
  _PhiTPhi[2] = _PhiTPhi[1];
  _PhiTPhi[3] = _length;


  if (_PhiTPhi[1]*_PhiTPhi[1] == _PhiTPhi[0]*_PhiTPhi[3])
    {
      // rank 1 (can not be rank 0, because _PhiTPhi[3]!=0)
      _bbdd = pow(_PhiTPhi[0]+_PhiTPhi[3],2);
      _a = (_PhiTPhi[0]*_PhiTS[0] + _PhiTPhi[1]*_PhiTS[1])/_bbdd;
      _c = (_PhiTPhi[1]*_PhiTS[0] + _PhiTPhi[3]*_PhiTS[1])/_bbdd;
    }
  else
    {
      // full rank
      _adbc = _PhiTPhi[0]*_PhiTPhi[3] - _PhiTPhi[1]*_PhiTPhi[1];
      _a = (_PhiTPhi[3]*_PhiTS[0] - _PhiTPhi[1]*_PhiTS[1])/_adbc;
      _c = (-_PhiTPhi[2]*_PhiTS[0] + _PhiTPhi[0]*_PhiTS[1])/_adbc;
    }
  
}


void VpabcFit::setInitPoint()
{
  x[0] = 0;
  int n = 0;

  if ( _S[_startIdx] <= 0 )
    { // can not log
      for ( int i = 0; i < _length; i++ )
	{
	  if (_S[_startIdx+i] > 0)
	    {
	      x[0] += (_S[_startIdx+i]/(i+1)); // version 1
	      n++;
	    }
	}
    }
  else
    {
      for ( int i = 0; i < _length-1; i++ )
	{
	  if (_S[_startIdx+i+1] > 0)
	    {
	      x[0] -= ( log(_S[_startIdx]/_S[_startIdx+i+1]) / _logT[i+1] ); // see comments of ```power2start``` function in curvefit toolbox (MATLAB)
	      n++;
	    }
	}

    }
  
  if ( n > 2 )
    {
      x[0] /= n;
    }
  else
    {
      x[0] = 0.5;
    }

}

void VpabcFit::setScale(double s)
{
  _scale = s;

  double rate = 0;
  
  if ( _length != 0 )
    {
      double maxS = 0;
      for ( int i = 0; i < _length; i++ )
	{
	  if ( _S[i] > maxS )
	    {
	      maxS = _S[i];
	    }
	}
      
      if ( maxS != 0 )
	{
	  rate = s/maxS;
	}

      for ( int i = 0; i < _length; i++ )
	{
	  _S[i] *= rate;
	}
    }
    
}


void VpabcFit::dump()
{
  for ( int i=0; i < _length; i++ )
    {
      cout << _S[_startIdx+i] << ' ';
    }
  cout << '\n';
  cout << "_length: " << _length << "\ta: " << _a << "\tb: " << _b << "\tc: " << _c << endl;
}
