#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

othello_bd *new_othello_bd (void) {
  othello_bd *bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  memset(bd->board,0,X_SIZE*Y_SIZE*sizeof(char));
  bd->board[3][3] = bd->board[4][4] = 1;
  bd->board[3][4] = bd->board[4][3] = -1;
  bd->turn = 1;
  bd->just_passed = false;
  return bd;
}

othello_bd *copy_othello_bd (othello_bd *old_bd) {
  othello_bd *new_bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  memcpy(new_bd,old_bd,sizeof(struct othello_bd));
  return new_bd;
}

void free_othello_bd (othello_bd *bd) {
  free((void*)bd);
}

double score (othello_bd *bd) {
  double result = 0;
  int squares = 0;
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      result += bd->board[i][j];
      if (bd->board[i][j]) {
        ++squares;
      }
    }
  }
  return result;
}

int distance_from_edge (int x, int y) {
  int result = x;
  int choices[3] = { y, 7-x, 7-y };
  for (int k = 0 ; k < 3 ; ++k) {
    if (choices[k] < result) {
      result = choices[k];
    }
  }
  return result;
}
