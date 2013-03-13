#include "ai.h"
#include "learning-ai.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LEARNING_RATE 1e-3
#define REGULARIZATION_FACTOR 1e-5

void init_learning_ai_game_state (learning_ai_game_state *lags) {
  lags->turns = 0;
}

void add_board (learning_ai_game_state *lags, othello_bd *bd) {
  assert (lags->turns <= 128);
  lags->cts[lags->turns] = compute_board_counts(bd);
  ++lags->turns;
}

void init_weights (learning_ai_weights *wts) {
  for (int k = 0 ; k < NUM_SQUARE_CLASSES  ; ++k) {
    wts->count_weights[k] = 0;
    wts->count_times_filled_weights[k] = 0;
  }
  printf("INIT WEIGHTS ");
  print_weights(wts);
}

double square (double x) {
  return x*x;
}

void print_learning_ai_game_state (learning_ai_game_state *lags, int winner) {
  for (int k = 0 ; k < lags->turns ; ++k) {
    fprintf(stderr,"%d %d %d %d %d %d %d\n",lags->cts[k].turn,lags->cts[k].squares_filled,lags->cts[k].counts[0],lags->cts[k].counts[1],lags->cts[k].counts[2],lags->cts[k].counts[3],winner);
  }
}

double sigmoid (double x) {
  return 1/(1+exp(-x));
}

double sigmoidPrime (double x) {
  /*
  if (x > 1000) {
    x = 1000;
  } else if (x < -1000) {
    x = -1000;
  }
  double ex = exp(-x);
  double result = 2*ex/((1+ex)*(1+ex));
  */
  double y = sigmoid(x);
  return 2*(1-y)*(y+1);
  //assert(!isnan(result));
  //return result;
}

double weight_board_counts (learning_ai_weights *wts,board_counts *cts) {
  double result = 0;
  for (int k = 0 ; k < NUM_SQUARE_CLASSES ; ++k) {
    result += wts->count_times_filled_weights[k] * cts->counts[k] * sqrt(1-cts->squares_filled/64.0);
    result += wts->count_weights[k] * cts->counts[k];
  }
  //printf("result");
  //print_weights(wts);
  //printf("result:%f\n",result);
  return result;
}

void update_weights_from_counts (learning_ai_weights *wts,board_counts *cts,int winner) {
  double linsum = weight_board_counts (wts,cts);
  // double winner_predicted = sigmoid(linsum);
  double factor = sigmoid(-linsum*winner)*winner; // 2*sigmoidPrime(linsum)*(winner_predicted-winner);
  for (int k = 0 ; k < NUM_SQUARE_CLASSES ; ++k) {
    wts->count_weights[k] += LEARNING_RATE * factor * cts->counts[k] - REGULARIZATION_FACTOR*wts->count_weights[k];
    wts->count_times_filled_weights[k] += LEARNING_RATE * factor * cts->counts[k] * sqrt(1-cts->squares_filled/64.0) - REGULARIZATION_FACTOR*wts->count_times_filled_weights[k];
    // wts->count_times_filled_weights[k] -= LEARNING_RATE * factor * cts->counts[k] * cts->squares_filled
                                          // + REGULARIZATION_FACTOR*wts->count_times_filled_weights[k];
  }
}

void update_weights_from_game (learning_ai_weights *wts, learning_ai_game_state *lags, int winner, int n) {
  for (int rep = 0 ; rep < n ; ++rep) {
    for (int k = 0 ; k < lags->turns ; ++k) {
      update_weights_from_counts(wts,&lags->cts[k],winner);
    }
  }
}

void print_weights (learning_ai_weights *wts) {
  for (int k = 0 ; k < NUM_SQUARE_CLASSES; ++k) {
    printf("%d: %f %f /",k,wts->count_weights[k],wts->count_times_filled_weights[k]);
  }
}
