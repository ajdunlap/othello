#ifndef LEARNING_AI_H
#define LEARNING_AI_H
#include "ai.h"
#include "board.h"

typedef struct learning_ai_weights {
  double count_weights[NUM_SQUARE_CLASSES];
  double count_times_filled_weights[NUM_SQUARE_CLASSES];
} learning_ai_weights;

typedef struct learning_ai_game_state {
  int turns;
  board_counts cts[2*X_SIZE*Y_SIZE];
} learning_ai_game_state;

void init_learning_ai_game_state (learning_ai_game_state *lags);

void add_board (learning_ai_game_state *lags, othello_bd *bd);

void print_learning_ai_game_state (learning_ai_game_state *lags, int winner);

double weight_board_counts (learning_ai_weights*,board_counts*);
double sigmoid (double);
double sigmoidPrime (double);

void update_weights_from_counts (learning_ai_weights *wts,board_counts *cts,int winner);
void update_weights_from_game (learning_ai_weights *wts, learning_ai_game_state *lags,int winner, int n);

void print_weights (learning_ai_weights *wts);

void init_weights (learning_ai_weights *wts);

#endif
