all: BlocksworldAStar ;

BlocksworldAStar: BlocksworldAStar.o Node.o State.o
	g++ -o BlocksworldAStar BlocksworldAStar.o Node.o State.o

BlocksworldAStar.o: BlocksworldAStar.cpp
	g++ -c BlocksworldAStar.cpp

Node.o: Node.cpp Node.h
	g++ -c Node.cpp

State.o: State.cpp State.h
	g++ -c State.cpp

clean:
	rm -f *.o