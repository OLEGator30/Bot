CXX= g++
CXXFLAGS= -g -Wall

SRCMODULES= common/common.cpp scanner/scanner.cpp buffer/buffer.cpp errors/errors.cpp parser/parser.cpp poliz/poliz.cpp functions/functions.cpp
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
	rm -f Prog deps.mk *~ buffer/*.o buffer/*~ scanner/*.o scanner/*~ errors/*.o errors/*~ parser/*.o parser/*~ common/*~ common/*.o poliz/*~ poliz/*.o functions/*.o functions/*~
