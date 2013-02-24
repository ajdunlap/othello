// x is rows
// y is columns

#ifndef BOARD_H
#define BOARD_H
#define X_SIZE 8
#define Y_SIZE 8

typedef struct othello_bd {
  char board[X_SIZE][Y_SIZE];
  char turn;
  char just_passed;
} othello_bd;

othello_bd *new_othello_bd (void);

othello_bd *copy_othello_bd (othello_bd *old_bd);

void free_othello_bd (othello_bd *bd);
#endif
