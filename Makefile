CXX= g++
CXXFLAGS= -g -Wall

SRCMODULES= scaner/scaner.cpp other/buffer.cpp 
OBJMODULES= $(SRCMODULES:.cpp=.o)

default: Prog

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Prog: main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
deps.mk: $(SRCMODULES)
	$(CXX) -MM $^ > $@

clean:
	rm -f Prog other/*.o  other/*~ scaner/*.o scaner/*~
