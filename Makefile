default: Prog

scaner.o: scaner/scaner.cpp scaner/scaner.hpp other/buffer.cpp other/buffer.hpp other/list.cpp other/list.hpp
	g++ -g -Wall -c scaner/scaner.cpp -o scaner/scaner.o

list.o: other/buffer.cpp other/buffer.hpp other/list.cpp other/list.hpp
	g++ -g -Wall -c other/list.cpp -o other/list.o

buffer.o: other/buffer.cpp other/buffer.hpp
	g++ -g -Wall -c other/buffer.cpp -o other/buffer.o

Prog: scaner.o buffer.o list.o
	g++ -g -Wall scaner/scaner.o other/buffer.o other/list.o main.cpp -o Prog

clean:
	rm -f Prog other/*.o  other/*~ scaner/*.o scaner/*~
