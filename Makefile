CXX= g++
CXXFLAGS= -g -Wall

SRCMODULES= scaner/scaner.cpp buffer/buffer.cpp errors/errors.cpp	parser/parser.cpp
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
	rm -f Prog deps.mk buffer/*.o  buffer/*~ scaner/*.o scaner/*~ errors/*.o errors/*~ parser/*.o parser/*~
