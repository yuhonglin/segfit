SegFit
=========

SegFit is a C++ implementation of the *SegFit* Algorithm proposed by the following paper,


>[Honglin Yu](http://yuhonglin.github.io/), [Lexing Xie](http://users.cecs.anu.edu.au/~xlx/) and [Scott Sanner](http://users.cecs.anu.edu.au/~ssanner/), **Exploring the Popularity Phases of YouTube Videos: Detection, Observations and Predictions**, Accepted by ICWSM-15



The goal is to segment a sequence of numbers into consecutive of phases and fit each of them with a generalized power-law,


<img style="display: block; margin-left: auto; margin-right: auto;" src="http://www.sciweavers.org/upload/Tex2Img_1426051911/render.png">



This repository also includes an easy-to-use python wrapper.

Version
-------
2.1

Compilation
-----------
Curently, the software are only tested under Linux (specifically, Ubuntu)
1. first make sure ```g++```, ```gfortran``` and ```cmake``` are installed.
2. Optional: If you need python support, install ```libboost-python-dev``` (tested under version >=1.54)
3. Then run ```cmake .``` in the folder. If no error happened, the executable is ```./segfit```. Python lib file is ```./libpysegfit.so```


Usage
----
###Main routine:###

1. run ```./segfit --help``` to see all the options. Main options are
  * ```-s```: the input sequence. Numbers can be separated by either blank or ','
  * ```-l```: how long of the input sequence from the beginning is to be segmented. For example, if running ```./segfit -s "1,2,3,4,5,6" -l 5``` will only segment the first 5 element of the input sequence, i.e. ```"1,2,3,4,5"```.
  * ```-smp```: the penalty used to avoid "over-segmnetation" (\eta in the paper), default value is 2.3

2. Output format: 
   * Each row is a segment(phase) found consecutively from left to right
   * In each row, the meanings of each number are (from left to right) "start index", "end index", "parameter a", "parameter b", "parameter c" and "fitting order". For example, if we run ```./segfit -s "1,2,3,4,5,1,4,9,16" -l 9```, the output can be seen as,
   
|            | start index | end index | parameter a | parameter b | parameter c| fitting order |
|:----------:|:-----------:|:---------:|:-----------:|:-----------:|:----------:|:-------------:|
| 1st phase: |      0      |     4     |    6.25     |      1      |      0     |       0       |
| 2rd phase: |      5      |     8     |    6.25     |      2      |      0     |       0       |

We can see the algorithm segments the sequence into two phases. Please notice that, the program scaled the sequnce to [0,100], so the parametes actually describe sequcence [6.25, 12.5, 18.75, 25.0, 31.25, 6.25, 25.0, 56.25, 100.0].


###Python library###

First, you need to put the ```libpysegfit.so``` to somewhere in the ```PYTHONPATH```. Here is an example

```python
>>> from libpysegfit import PySegFit
>>> psf = PySegFit()
>>> psf.get_para() # see the default parameters
{'lb': -6.0, 'btype': 2, 'm': 15.0, 'smp': 2.3, 'maxiter': 1000.0, 'factr': 5000.0, 'pgtol': 1e-05, 'ub': 6.0}
>>> psf.segfit([1,2,3,4,5,1,4,9,16]) # segfit a sequence
[(1, 5, 6.25, 1.0, 0.0, 0), (6, 9, 6.25, 2.0, 0.0, 0)]
```


   

###As a C++ library###
TODO
   
Copy Right Notice
-----------------
This piece of code is written by [*Honglin Yu*](http://yuhonglin.github.io/), except for the following files,

1. ```lib/solver.f``` : by [*Ciyou Zhu*](http://users.iems.northwestern.edu/~nocedal/lbfgsb.html) et al.
2. ```lib/program.h(cpp)``` : originally by [*Peter Carbonetto*](http://www.cs.ubc.ca/~pcarbo/lbfgsb-for-matlab.html)

LICENSE
-------
GPLv3

