src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

CC = g++

../segfit: $(obj) solver.o
	$(CC) -O3 -o $@ $^ -lgfortran

solver.o: solver.f
	gfortran -O3 -fexceptions -c solver.f

%.o: %.cpp
	g++ -O3 -Wall -std=c++11 -o $@ -c $< -lgfortran


.PHONY: clean
clean:
	rm -f $(obj)
