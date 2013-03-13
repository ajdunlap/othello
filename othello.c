#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "show.h"
#include "play.h"
#include "ai.h"
#include "random-ai.h"
#include "learning-ai.h"

#define MINIMAX_DEPTH_1 4
#define MINIMAX_DEPTH_2 3

minimax_node *trees[2] = { NULL, NULL };
learning_ai_weights weights;

double learning_static_eval (othello_bd *bd) {
  board_counts cts = compute_board_counts(bd);
  return weight_board_counts(&weights,&cts);
}

bool play_human_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y) {
  if (have_legal_moves(bd)) {
    while (true) {
      printf("Move for player %d? > ",bd->turn);
      if (scanf("%d %d",x,y) != 2) {
        while (getchar() != '\n'); // throw away this line
        printf("Bad move format; enter x y.\n");
      } else if (!play_piece_if_legal(bd,*x,*y)) {
        printf("Illegal move\n");
      } else {
        return 0;
      }
    }
  } else {
    printf("Player %d passes.\n",bd->turn);
    *x = *y = -1;
    return pass_turn_is_game_over(bd);
  }
}

bool play_random_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y) {
  // show_othello_bd(stdout,bd);
  random_move(bd,x,y);
  if (*x != -1) {
    printf("I played %d %d\n",*x,*y);
    assert(play_piece_if_legal(bd,*x,*y));
    return false;
  } else {
    printf("Player %d passes.\n",bd->turn);
    *x = *y = -1;
    return pass_turn_is_game_over(bd);
  }
}

bool play_minimax_ai_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y) {
  bool game_over;
  int i = (1+bd->turn)>>1;
  if (trees[i]) {
    trees[i] = cut_tree_for_move(trees[i],opp_x,opp_y);
    assert(!trees[i] || boards_equal(trees[i]->bd,bd));
  }
  trees[i] = build_minimax_tree(bd->turn == 1 ? static_eval : learning_static_eval,trees[i],0,bd,bd->turn == 1 ? MINIMAX_DEPTH_1 : MINIMAX_DEPTH_2);
  if (have_legal_moves(bd)) {
    printf("Static evaluation: %f\n",learning_static_eval(bd));
    best_move(trees[i],x,y);
    printf("I played %d %d\n",*x,*y);
    int result = play_piece_if_legal(bd,*x,*y);
    assert(result);
    game_over = false;
  } else {
    printf("Player %d passes.\n",bd->turn);
    *x = *y = -1;
    // show_othello_bd(stdout,bd);
    game_over = pass_turn_is_game_over(bd);
  }
  if (!game_over) {
    trees[i] = cut_tree_for_move(trees[i],*x,*y);
    assert(!trees[i] || boards_equal(trees[i]->bd,bd));
  }
  return game_over;
}

int main (int argc, char **argv) {
  // initialize random number generator
  struct timeval cur;
  gettimeofday(&cur,NULL);
  srand(cur.tv_usec);
  printf("%d\n",(int)cur.tv_usec);

  // functions to play the game
  bool (*play[2]) (othello_bd*,int*,int*,int,int);

  // how many games should we play?
  int ngames = 1;

  // parse command line
  if (argc >= 3) {
    for (int k = 0 ; k < 2 ; ++k) {
      if (!strcmp(argv[k+1],"human")) {
        play[k] = play_human_turn_is_game_over;
      } else if (!strcmp(argv[k+1],"cpu")) {
        play[k] = play_minimax_ai_turn_is_game_over;
      } else if (!strcmp(argv[k+1],"random")) {
        play[k] = play_random_turn_is_game_over;
      } else {
        fprintf(stderr,"Player name must be 'human' or 'cpu'.");
        exit(1);
      }
    }
    if (argc >= 4) {
      ngames = atoi(argv[3]);
    }
  } else {
    play[0] = play_human_turn_is_game_over;
    play[1] = play_minimax_ai_turn_is_game_over;
  }

  printf("ngames: %d",ngames);

  init_weights(&weights);

  int wins1 = 0, wins2 = 0, wins1backhalf = 0, wins2backhalf = 0;

  learning_ai_game_state *lags_arr = (learning_ai_game_state*)calloc(ngames,sizeof(learning_ai_game_state));
  for (int game = 0 ; game < ngames ; ++game) {
    othello_bd the_bd;
    othello_bd *bd = &the_bd;
    // initialize learning AI machinery in case a function ends up using it
    init_learning_ai_game_state(&lags_arr[game]);

    reset_othello_bd(bd);
    bool game_over = false;
    while (!game_over) {
      show_othello_bd(stdout,bd);
      int x,y;
      game_over = (play[(1-bd->turn)>>1])(bd,&x,&y,x,y);
      add_board(&lags_arr[game],bd);
    }
    show_othello_bd(stdout,bd);

    double winner = score(bd);

    if (winner > 0) {
      printf("Player 1 wins!\n");
      if (game > ngames/2) {
        ++wins1backhalf;
      }
      ++wins1;
    } else if (winner < 0) {
      printf("Player -1 wins!\n");
      ++wins2;
      if (game > ngames/2) {
        ++wins2backhalf;
      }
    } else {
      printf("Tie!\n");
    }
    for (int k = 0 ; k < 2 ; ++k) {
      if (trees[k]) {
        free_minimax_tree(trees[k]);
        trees[k] = NULL;
      }
    }
    print_learning_ai_game_state(&lags_arr[game],winner ? winner / abs(winner) : 0);
    for (int l = 0 ; l < 10 ; ++l) {
      for (int k = game >= 0 ? game : 0 ; k <= game ; ++k) {
        update_weights_from_game(&weights,&lags_arr[k],winner,1);
      }
    }
    printf("%2d WEIGHTS ",winner > 0);
    print_weights(&weights);
    printf("SCORE: 1: %d (%d); 2: %d (%d)\n",wins1,wins1backhalf,wins2,wins2backhalf);
  }

  exit(0);
}
