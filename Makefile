# configuration
export CXX = g++
export FORTRAN = gfortran
export CXXFLAGS = -std=c++11 -O3 -Wall -pedantic
export LDFLAGS = -lgfortran
export FORFLAGS = $(CXXFLAGS)
# makefile
.PHONY: segfit
segfit:
	@$(MAKE) -C src

.PHONY: clean
clean:
	@rm ./segfit
	@$(MAKE) clean -C src
