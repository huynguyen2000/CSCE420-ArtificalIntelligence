all: BlocksworldBFS ;

BlocksworldBFS: BlocksworldBFS.o Node.o State.o
	g++ -o BlocksworldBFS BlocksworldBFS.o Node.o State.o

BlocksworldBFS.o: BlocksworldBFS.cpp
	g++ -c BlocksworldBFS.cpp

Node.o: Node.cpp Node.h
	g++ -c Node.cpp

State.o: State.cpp State.h
	g++ -c State.cpp

clean:
	rm -f *.o