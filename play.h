#ifndef PLAY_H
#define PLAY_H
#include "board.h"
int play_piece_or_legal_move (int play,othello_bd *bd,int x,int y);

int play_piece_if_legal (othello_bd *bd,int x,int y);

int legal_move (othello_bd *bd,int x,int y);

int have_legal_moves (othello_bd *bd);

int pass_turn_is_game_over (othello_bd *bd);
#endif
