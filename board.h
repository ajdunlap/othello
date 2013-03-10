// x is rows
// y is columns

#ifndef BOARD_H
#define BOARD_H
#define X_SIZE 8
#define Y_SIZE 8
#include <stdbool.h>

typedef struct othello_bd {
  char board[X_SIZE][Y_SIZE];
  char turn;
  char just_passed;
} othello_bd;

othello_bd *new_othello_bd (void);

void copy_othello_bd (othello_bd *new_bd, othello_bd *old_bd);
othello_bd *new_othello_bd_copy (othello_bd *old_bd);

void free_othello_bd (othello_bd *bd);

double score (othello_bd *bd);

int distance_from_edge (int x, int y);

bool boards_equal (othello_bd *b1,othello_bd *b2);
#endif
