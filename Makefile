CFLAGS=-std=c99 -g -Wall
LDFLAGS=-lm

SRC=board.c show.c play.c othello.c ai.c fringe.c
OBJ=board.o show.o play.o othello.o ai.o fringe.o

othello: ${OBJ}
	${CC} -o othello ${OBJ} ${LDFLAGS}

play.o: board.h
othello.o: show.h board.h play.h ai.h
show.o: board.h
ai.o: board.h play.h show.h fringe.h
fringe.o: fringe.h ai.h

clean:
	rm -f ${OBJ} othello
