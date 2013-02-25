#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

void show_othello_piece(FILE *f,char c, int last_played) {
  switch (c) {
    case -1:
      fprintf(f,"\e[%smX\e[0m|", last_played? "1;41" : "31");
      break;
    case 0:
      fprintf(f," |");
      break;
    case 1:
      fprintf(f,"\e[%smO\e[0m|", last_played? "1;44" : "34");
      break;
    default:
      break;
  }
}

void print_horiz_border (FILE *f) {
  fprintf(f," +");
  for (int j = 0 ; j < X_SIZE*2-1 ; ++j) {
    fprintf(f,"-");
  }
  fprintf(f,"+\n");
}

void show_othello_bd_with_last_move (FILE *f,othello_bd *bd, int last_x, int last_y) {
  fprintf(f," |");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    fprintf(f,"%d|",j);
  }
  fprintf(f,"\n");
  print_horiz_border(f);
  for (int i = 0 ; i < Y_SIZE ; ++i) {
    fprintf(f,"%d|",i);
    for (int j = 0 ; j < X_SIZE ; ++j) {
      show_othello_piece(f,bd->board[i][j], j==last_y && i==last_x);
    }
    fprintf(f,"\n");
  }
  print_horiz_border(f);
}
void show_othello_bd (FILE *f,othello_bd *bd) {
    show_othello_bd_with_last_move(f, bd, -1, -1);
}
