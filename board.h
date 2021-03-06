// x is rows
// y is columns

#ifndef BOARD_H
#define BOARD_H
#define X_SIZE 8
#define Y_SIZE 8
#define NUM_SQUARE_CLASSES 10
#include <stdbool.h>

typedef struct othello_bd {
  char board[X_SIZE][Y_SIZE];
  char turn;
  char just_passed; // the last player passed
} othello_bd;

othello_bd *new_othello_bd (void);
void reset_othello_bd (othello_bd *bd);

void copy_othello_bd (othello_bd *new_bd, othello_bd *old_bd);
othello_bd *new_othello_bd_copy (othello_bd *old_bd);

void free_othello_bd (othello_bd *bd);

double score (othello_bd *bd);

int type_hand_score(int t);
int distance_from_edge (int x, int y);
int square_type (int x, int y);

bool boards_equal (othello_bd *b1,othello_bd *b2);
#endif
