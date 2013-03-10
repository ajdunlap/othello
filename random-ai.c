#include "play.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

void random_move (othello_bd *bd, int *x, int *y) {
  int sofar = 0;
  struct timeval cur;
  gettimeofday(&cur,NULL);
  srand(cur.tv_usec);
  bool has_move = false;
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      if (legal_move(bd,i,j)) {
        has_move = true;
        ++sofar;
        int n = rand() % sofar;
        if (0 == n) {
          *x = i;
          *y = j;
        }
      }
    }
  }
  if (!has_move) {
    *x = -1;
    *y = -1;
  }
}
