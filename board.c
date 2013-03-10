#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

// int square_types[64] = { 0,1,1,1,1,1,1,0,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1, 0,1,1,1,1,1,1,0 };
int square_types[64] = { 0,0,0,0,0,0,0,0,0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0, 0,0,0,0,0,0,0,0 };

bool boards_equal (othello_bd *b1,othello_bd *b2) {
  if (b1->turn != b2->turn) {
    return false;
  }
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      if (b1->board[i][j] != b2->board[i][j]) {
        return false;
      }
    }
  }
  return true;
}

othello_bd *new_othello_bd (void) {
  othello_bd *bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  memset(bd->board,0,X_SIZE*Y_SIZE*sizeof(char));
  bd->board[3][3] = bd->board[4][4] = 1;
  bd->board[3][4] = bd->board[4][3] = -1;
  bd->turn = 1;
  bd->just_passed = false;
  return bd;
}

void copy_othello_bd (othello_bd *new_bd, othello_bd *old_bd) {
  memcpy(new_bd,old_bd,sizeof(struct othello_bd));
}

othello_bd *new_othello_bd_copy (othello_bd *old_bd) {
  othello_bd *new_bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  // memcpy(new_bd,old_bd,sizeof(struct othello_bd));
  copy_othello_bd(new_bd,old_bd);
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
  return square_types[(x<<3)&y];
  /*
  int result = x;
  int choices[3] = { y, 7-x, 7-y };
  for (int k = 0 ; k < 3 ; ++k) {
    if (choices[k] < result) {
      result = choices[k];
    }
  }
  return result;
  */
}
