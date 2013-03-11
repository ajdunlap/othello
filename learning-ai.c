#include "ai.h"
#include "learning-ai.h"
#include <assert.h>
#include <stdio.h>

void init_learning_ai_game_state (learning_ai_game_state *lags) {
  lags->turn_index = 0;
}

void add_board (learning_ai_game_state *lags, othello_bd *bd) {
  assert (lags->turn_index <= 128);
  lags->cts[lags->turn_index] = compute_board_counts(bd);
  lags->turn_index++;
}

void print_learning_ai_game_state (learning_ai_game_state *lags, int winner) {
  for (int k = 0 ; k < lags->turn_index ; ++k) {
    fprintf(stderr,"%d %d %d %d %d %d %d\n",lags->cts[k].turn,lags->cts[k].squares_filled,lags->cts[k].counts[0],lags->cts[k].counts[1],lags->cts[k].counts[2],lags->cts[k].counts[3],winner);
  }
}
