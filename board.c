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
