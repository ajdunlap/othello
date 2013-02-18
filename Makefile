CFLAGS=-std=c99 -g
LDFLAGS=-lm

SRC=board.c show.c play.c othello.c ai.c
OBJ=board.o show.o play.o othello.o ai.o

othello: ${OBJ}

play.o: board.h
main.o: show.h board.h play.h ai.h
show.o: board.h
ai.o: board.h play.h

clean:
	rm -f ${OBJ} othello
