CFLAGS=-std=c99 -Wall -O2 # -pg
LDFLAGS=-lm # -pg

SRC=board.c show.c play.c othello.c ai.c learning-ai.c random-ai.c
OBJ=board.o show.o play.o othello.o ai.o learning-ai.o random-ai.o

othello: ${OBJ}
	${CC} -o othello ${OBJ} ${LDFLAGS}

play.o: board.h
othello.o: show.h board.h play.h ai.h learning-ai.h random-ai.h
show.o: board.h
ai.o: board.h play.h show.h
learning-ai.o: learning-ai.h ai.h board.h
random-ai.o: random-ai.h board.h play.h

clean:
	rm -f ${OBJ} othello
