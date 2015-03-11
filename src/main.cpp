/**
 * @file   main.cpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Sun Dec 22 10:39:41 2013
 * 
 * @brief  SegFit: an algorithm combining segmentation and fitting
 * 
 * 
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <list>
#include <string>
#include <sstream>

#include <cstdlib>
#include <cmath>

#ifdef DEBUG
#include <fenv.h>
#endif

#include "option.hpp"
#include "segFit.hpp"

using namespace std;

int main(int argc, char *argv[])
{

  
#ifdef DEBUG
  /**
   * function (log, pow) in cmath may lead to FE_INEXACT exceptions
   */
  feenableexcept(-1 xor FE_INEXACT);
#endif
  
  /* parse options */
  dms::Option opt;
  opt.set_head( "\n           _____            ______ _ _\n          / ____|          |  ____(_) |\n         | (___   ___  __ _| |__   _| |_\n          \\___ \\ / _ \\/ _` |  __| | | __|\n          ____) |  __/ (_| | |    | | |_\n         |_____/ \\___|\\__, |_|    |_|\\__|\n                       __/ |             \n                      |___/              (v 2.1)\n\nC++ implementation of segFit based on lbfgsb fortran library\n\nRelease note:\n  -  (v 2.0) This version fit every segment twice in different orders and pick the order with smaller SSE\n  -  (v 2.1) change input/output format\n\nAbout options:\n" );
  opt.set_tail("\n");
  opt.add_string("-s", "the sequence to process");
  opt.add_int("-l", "analyse first l-length strings");
  opt.add_double("-smp", "switch model penalty", 2.3);
  set< int > optbt;
  optbt.insert(0);
  optbt.insert(1);
  optbt.insert(2);
  opt.add_int("-btype", "bound types (0,1,2)", 2, optbt);
  opt.add_double("-lb", "lower bound of parameter b", -6);
  opt.add_double("-ub", "upper bound of parameter b", 6);
  opt.add_int("-m", "m", 15);
  opt.add_int("-maxiter", "max iteration", 1000);
  opt.add_int("-factr", "max function call", 5000);
  opt.add_double("-pgtol", "pgtol", 10e-5);
  
  switch (opt.parse(argc, argv))
    {
    case dms::Option::PARSE_HELP:
      return 0;
    case dms::Option::PARSE_ERROR:
      cerr << "option parsing error, use --help to for usage" << endl;
      return 1;
    default:
      break;
    }

  string sequence = opt.get_string("-s");
  int firstN = opt.get_int("-l");
  double smp = opt.get_double("-smp");
  int btype = opt.get_int("-btype");
  double lb = opt.get_double("-lb");
  double ub = opt.get_double("-ub");
  int m = opt.get_int("-m");
  int maxiter = opt.get_int("-maxiter");
  int factr = opt.get_int("-factr");
  double pgtol = opt.get_double("-pgtol");

  /**
   * sanity check of options
   * 
   */

  if ( firstN <= 0 )
    {
      cerr << "the value of -l option should be bigger than 0" << endl;
      return 1;
    }

  if (smp <= 0 )
    {
      cerr << "the value of -smp option should be bigger than 0" << endl;
      return 1;
    }

  if ( lb > ub )
    {
      cerr << "the value of option -lb should be smaller than that of -ub" << endl;
      return 1;
    }

  if ( m <= 0 )
    {
      cerr << "the value of option -m should be bigger than 0" << endl;
      return 1;
    }

  if ( maxiter <= 0 )
    {
      cerr << "the value of option -maxiter should be bigger than 0" << endl;
      return 1;
    }

  if ( factr <= 0 )
    {
      cerr << "the value of option -factr should be bigger than 0" << endl;
      return 1;
    }

  if ( pgtol <= 0 )
    {
      cerr << "the value of option -pgtol should be bigger than 0" << endl;
      return 1;
    }

  

  // convert the input string into a double array
  double * seqdbptr = new double[firstN];
  sequence.erase(sequence.find_last_not_of(" \n\r\t")+1);

  for ( int i = 0; i < sequence.size(); i ++ )
    {
      if ( sequence[i] == ',' or sequence[i] == '\t' or sequence[i] == '\n' or sequence[i] == ' ' )
	{
	  sequence[i] = ' ';
	  continue;
	}

      if ( sequence[i] > '9' or sequence[i] < '0' )
	{
	  cerr << "invalide charactor \"" << sequence[i] << "\" in the input sequence at position " << i << ", check the value after \"-s\" option" << endl;
	  exit(1);
	}
    }
  
  stringstream ss(sequence);

  for ( int i = 0; i < firstN; i ++ )
    {
      if (ss.eof())
	{
	  cerr << "Error: some viewcount in the viewcount file is less than acquired" << endl;
	  return 1;
	}
      ss >> seqdbptr[i];
    }

  // compute
  SegFit segFit(smp, btype, lb, ub, m, maxiter, factr, pgtol);
  const vector<Segment>* result;
  
  segFit.setString(seqdbptr, firstN);
  segFit.run();

  result = segFit.getResult();  
  
  for ( vector<Segment>::const_iterator iter = result->begin(); iter != result->end(); iter++ )
    {
      cout << iter->headIndex << '\t' << iter->tailIndex << '\t' << iter->a << '\t' << iter->b << '\t' << iter->c << '\t' << iter->order << endl;
    }
  
  delete[] seqdbptr;
  
  return 0;
}
