#ifndef LEARNING_AI_H
#define LEARNING_AI_H
#include "ai.h"

typedef struct learning_ai_game_state {
  int turn_index;
  board_counts cts[2*X_SIZE*Y_SIZE];
} learning_ai_game_state;

void init_learning_ai_game_state (learning_ai_game_state *lags);

void add_board (learning_ai_game_state *lags, othello_bd *bd);

void print_learning_ai_game_state (learning_ai_game_state *lags, int winner);
#endif
