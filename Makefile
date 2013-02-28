CFLAGS=-std=c99 -g -Wall
LDFLAGS=-lm

SRC=board.c show.c play.c othello.c ai.c fringe-heap.c
OBJ=board.o show.o play.o othello.o ai.o fringe-heap.o

othello: ${OBJ}

play.o: board.h
othello.o: show.h board.h play.h ai.h
show.o: board.h
ai.o: board.h play.h show.h
fringe-heap.o: fringe-heap.h ai.h

clean:
	rm -f ${OBJ} othello
