all: sammys test ;

sammys: sammys.o parser.o NatDed.o
	g++ -std=c++11 -o sammys sammys.o parser.o NatDed.o

test: test.o parser.o NatDed.o
	g++ -std=c++11 -o test test.o parser.o NatDed.o

sammys.o: sammys.cpp
	g++ -std=c++11 -c sammys.cpp

test.o: test.cpp
	g++ -std=c++11 -c test.cpp

parser.o: parser.cpp parser.h
	g++ -std=c++11 -c parser.cpp

NatDed.o: NatDed.cpp NatDed.h
	g++ -std=c++11 -c NatDed.cpp

clean:
	rm -f *.o
