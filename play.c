#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

int play_piece_or_legal_move (int play,othello_bd *bd,int x,int y) {
  if (bd->board[x][y]) {
    return 0;
  }
  int legal_move = 0;
  for (int x_dir = -1 ; x_dir <= 1 ; ++x_dir) {
    for (int y_dir = -1 ; y_dir <= 1 ; y_dir += (x_dir ? 1 : 2)) { // skip 0 if x_dir is 0
      char max_x = x;
      char max_y = y;
      char on_board = 1;
      do {
        max_x += x_dir;
        max_y += y_dir;
      } while ((on_board = (max_x >= 0 && max_y >= 0 && max_x < X_SIZE && max_y < Y_SIZE)) && bd->board[max_x][max_y] == -bd->turn);
      if (on_board && bd->board[max_x][max_y] == bd->turn) {
        int cur_x = x+x_dir;
        int cur_y = y+y_dir;
        for ( ; cur_x*x_dir < max_x*x_dir || cur_y*y_dir < max_y*y_dir ; cur_x += x_dir, cur_y += y_dir) {
          if (play) {
            legal_move = 1;
            bd->board[cur_x][cur_y] = bd->turn;
          } else {
            return 1;
          }
        }
      }
    }
  }
  if (play && legal_move) {
    bd->just_passed = false;
    bd->board[x][y] = bd->turn;
    bd->turn = -(bd->turn);
  }
  return legal_move;
}

int play_piece_if_legal (othello_bd *bd,int x,int y) {
  return play_piece_or_legal_move (1,bd,x,y);
}

int legal_move (othello_bd *bd,int x,int y) {
  return play_piece_or_legal_move(0,bd,x,y);
}

int have_legal_moves (othello_bd *bd) {
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      if (legal_move(bd,i,j)) {
        return true;
      }
    }
  }
  return false;
}

int pass_turn_is_game_over (othello_bd *bd) {
  bd->turn = -(bd->turn);
  if (bd->just_passed) {
    return 1;
  } else {
    bd->just_passed = true;
    return 0;
  }
}
