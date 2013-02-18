#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

void show_othello_piece(char c) {
  switch (c) {
    case -1:
      printf("X");
      break;
    case 0:
      printf(" ");
      break;
    case 1:
      printf("O");
      break;
    default:
      break;
  }
}

void print_horiz_border (void) {
  printf(" +");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    printf("-");
  }
  printf("+\n");
}

void show_othello_bd (othello_bd *bd) {
  printf("  ");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    printf("%d",j);
  }
  printf("\n");
  print_horiz_border();
  for (int i = 0 ; i < Y_SIZE ; ++i) {
    printf("%d|",i);
    for (int j = 0 ; j < X_SIZE ; ++j) {
      show_othello_piece(bd->board[i][j]);
    }
    printf("|\n");
  }
  print_horiz_border();
}
