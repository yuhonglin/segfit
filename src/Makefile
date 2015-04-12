src = $(wildcard *.cpp)
header = $(wildcard *.hpp) $(wildcard *.h)
obj = $(src:.cpp=.o)

../segfit: $(obj) solver.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

solver.o: solver.f
	$(FORTRAN) $(FORFLAGS) -c solver.f

%.o: %.cpp %.hpp
	g++ $(CXXFLAGS) -o $@ -c $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj)
