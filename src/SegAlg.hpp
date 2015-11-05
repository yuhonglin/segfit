/**
 * @file   SegAlg.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Mon Apr  6 11:26:37 2015
 *
 * @brief  The algorithm base class
 *
 *
 */

#ifndef SEGALG_H
#define SEGALG_H

#include <vector>
#include <string>
#include <memory>

#include "FitAlg.hpp"
#include "FitAlgFactory.hpp"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::string;

/**
 * The algorithm base class
 *
 */

class SegAlg {
public:
  SegAlg();
  virtual ~SegAlg();

  /// must be defined by subclass
  virtual void run() = 0;

  /// interface to dump information
  virtual void dump() = 0;

  /// set the string
  virtual void set_string(shared_ptr<vector<double> > &pd);

  /// set the fitting algorithm
  void set_fitalg(string n);

  /// the interface to set parameters
  virtual void set_parameter(string name, double value);
  virtual void set_parameter(string name, string value);
  virtual void set_parameter(string name, int value);

  /// get the result
  shared_ptr<vector<Segment> > get_result();

  /// set the number of segments
  int set_number_of_segment(int n);

protected:
  /// the string
  shared_ptr<vector<double> > _shrS;

  /// the scale
  double _scale;

  /// the segmented result
  shared_ptr<vector<Segment> > _result;

  /// FitAlg
  unique_ptr<FitAlg> _fitAlg;

  /// name
  string _name;

  /// number of segment
  int _number_of_segment;

  /// factory
  unique_ptr<FitAlgFactory> _fitAlgFactory;

}; // class Algorithm

#endif /* SEGALG_H */
