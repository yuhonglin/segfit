/**
 * @file   main.cpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Thur May 22 10:50:10 2014
 * 
 * @brief  pysegfit: the python wrap-up of segfit algorithm
 * 
 * 
 */

#include <iostream>
using std::cout;
using std::endl;

#include <vector>

#ifdef DEBUG
#include <fenv.h>
#endif


#include <boost/python.hpp>
using namespace boost::python;


#include "segFit.hpp"
#include "vpabcFit.hpp"


class PySegFit
{
public:
  PySegFit()
  {
    #ifdef DEBUG
    feenableexcept(-1 xor FE_INEXACT);
    #endif
    
    smp_ = 2.3;
    btype_ = 2;
    lb_ = -6;
    ub_ = 6;
    m_ = 15;
    maxiter_ = 1000;
    factr_ = 5000;
    pgtol_ = 1e-5;

    scale_ = 100;
    
    segfit_ = new SegFit(smp_, btype_, lb_, ub_, m_, maxiter_, factr_, pgtol_);

    vpabcFit_asc_ = new VpabcFit( &x0_, &lb_, &ub_, &btype_, m_, maxiter_, factr_, pgtol_);
    vpabcFit_des_ = new VpabcFit( &x0_, &lb_, &ub_, &btype_, m_, maxiter_, factr_, pgtol_);
    
  }
  
  ~PySegFit()
  {
    if (segfit_!=NULL)
      {
	delete segfit_;
      }
  }

  /// segfit a series
  list segfit(const list& l)
  {
    double * tmpPtr = new double[len(l)];
    for ( int i = 0; i < len(l); i ++ )
      {
	tmpPtr[i] = extract<double>(l[i]);
      }
    segfit_->setString(tmpPtr, len(l));
    segfit_->run();
    delete[] tmpPtr;

    const vector<Segment>* result = segfit_->getResult();

    list ret;
    for ( std::vector<Segment>::const_iterator iter = result->begin(); iter != result->end(); iter++ )
      	{
      	  ret.append(make_tuple(iter->headIndex+1, iter->tailIndex+1, iter->a, iter->b, iter->c, int(iter->order)));
      	}
    return ret;
  };

  /// fit a power law
  list vpabcFit(const list& l)
  {
    int segLen = len(l);
    int i = 0;
    
    double * tmpPtr = new double[len(l)];
    double maxValue = 0;
    for ( i = 0; i < segLen; i ++ )
      {
	tmpPtr[i] = extract<double>(l[i]);
	if (tmpPtr[i] > maxValue)
	  {
	    maxValue = tmpPtr[i];
	  }
      }

    vpabcFit_asc_->setString(tmpPtr, segLen);

    for ( i = 0; i < segLen; i ++ )
      {
	tmpPtr[segLen-i-1] = extract<double>(l[i]);
      }

    vpabcFit_des_->setString(tmpPtr, segLen);

    vpabcFit_asc_->runSolver();
    vpabcFit_des_->runSolver();

    double tmpPenalty = vpabcFit_asc_->getObjective();
    list ret;
    double parameter[3];

    double vcRate = maxValue/scale_;
    
    if (tmpPenalty <= (vpabcFit_des_->getObjective()))
      {
	vpabcFit_asc_->getParameter(parameter[0], parameter[1], parameter[2]);
	ret.append(parameter[0]*vcRate);
	ret.append(parameter[1]);
	ret.append(parameter[2]*vcRate);
	ret.append(vpabcFit_asc_->getObjective()*vcRate*vcRate);
	ret.append(0);
      }
    else
      {
	vpabcFit_des_->getParameter(parameter[0], parameter[1], parameter[2]);
	ret.append(parameter[0]*vcRate);
	ret.append(parameter[1]);
	ret.append(parameter[2]*vcRate);
	ret.append(vpabcFit_des_->getObjective()*vcRate*vcRate);
	ret.append(1);
      }
    
    return ret;
  }

  void setScale (double s)
  {
    segfit_->setScale(s);

    vpabcFit_asc_->setScale(s);
    vpabcFit_des_->setScale(s);
  }

  dict getPara()
  {

    dict ret;
    ret["smp"] = smp_;
    ret["btype"] = btype_;
    ret["lb"] = lb_;
    ret["ub"] = ub_;
    ret["m"] = m_;
    ret["maxiter"] = maxiter_;
    ret["factr"] = factr_;
    ret["pgtol"] = pgtol_;

    return ret;
  }

  void setSMP(double s)
  {
    smp_ = s;
    segfit_->setSwitchModelPenalty(s);
  }
  
private:

  double smp_;
  int btype_;
  double lb_;
  double ub_;
  double m_;
  double maxiter_;
  double factr_;
  double pgtol_;

  double scale_;
  
  SegFit * segfit_;

  VpabcFit * vpabcFit_asc_;
  VpabcFit * vpabcFit_des_;

  double x0_;
};


BOOST_PYTHON_MODULE(libpysegfit)
{
  class_<PySegFit>("PySegFit")
    .def("segfit", &PySegFit::segfit)
    .def("vpabcFit", &PySegFit::vpabcFit)
    .def("set_scale", &PySegFit::setScale)
    .def("get_para", &PySegFit::getPara)
    .def("set_smp", &PySegFit::setSMP)
    ;
}
