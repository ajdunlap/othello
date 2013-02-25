#ifndef SHOW_H
#define SHOW_H
void show_othello_piece(FILE *f,char c, int last_played);

void print_horiz_border (FILE *f);

void show_othello_bd_with_last_move (FILE *f,othello_bd *bd, int last_x, int last_y);
void show_othello_bd (FILE *f,othello_bd *bd);
#endif
