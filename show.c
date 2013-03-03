#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

void show_othello_piece(FILE *f,char c) {
  switch (c) {
    case -1:
      fprintf(f,"\e[31mX\e[0m|");
      break;
    case 0:
      fprintf(f," |");
      break;
    case 1:
      fprintf(f,"\e[34mO\e[0m|");
      break;
    default:
      break;
  }
}

void show_othello_piece_no_color(FILE *f,char c) {
  switch (c) {
    case -1:
      fprintf(f,"X|");
      break;
    case 0:
      fprintf(f," |");
      break;
    case 1:
      fprintf(f,"0|");
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

void show_othello_bd (FILE *f,othello_bd *bd) {
  fprintf(f," |");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    fprintf(f,"%d|",j);
  }
  fprintf(f,"\n");
  print_horiz_border(f);
  for (int i = 0 ; i < Y_SIZE ; ++i) {
    fprintf(f,"%d|",i);
    for (int j = 0 ; j < X_SIZE ; ++j) {
      show_othello_piece(f,bd->board[i][j]);
    }
    fprintf(f,"\n");
  }
  print_horiz_border(f);
}

void show_othello_bd_for_dot (FILE *f,othello_bd *bd) {
  fprintf(f," |");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    fprintf(f,"%d|",j);
  }
  fprintf(f,"\\n");
  for (int i = 0 ; i < Y_SIZE ; ++i) {
    fprintf(f,"%d|",i);
    for (int j = 0 ; j < X_SIZE ; ++j) {
      show_othello_piece_no_color(f,bd->board[i][j]);
    }
    fprintf(f,"\\n");
  }
}
