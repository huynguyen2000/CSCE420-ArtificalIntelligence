all: DPLL generateQueensClauses ;

generateQueensClauses: generateQueensClauses.o
	g++ -std=c++11 -o generateQueensClauses generateQueensClauses.o

generateQueensClauses.o: generateQueensClauses.cpp
	g++ -std=c++11 -c generateQueensClauses.cpp

DPLL: DPLL.o parser.o
	g++ -std=c++11 -o DPLL DPLL.o parser.o

DPLL.o: DPLL.cpp
	g++ -std=c++11 -c DPLL.cpp

parser.o: parser.cpp parser.h
	g++ -std=c++11 -c parser.cpp

clean:
	rm -f *.o
