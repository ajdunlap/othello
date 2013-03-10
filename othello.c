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

learning_ai_game_state lags;
minimax_node *trees[2] = { NULL, NULL };

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
    assert(boards_equal(trees[i]->bd,bd));
  }
  trees[i] = build_minimax_tree(trees[i],0,bd,bd->turn == 1 ? 4 : 4);
  if (have_legal_moves(bd)) {
    // show_othello_bd(stdout,bd);
    printf("Static evaluation: %f\n",static_eval(bd));
    printf("turn: %d, i: %d\n",bd->turn,i);
    //show_minimax_tree(node);
    best_move(trees[i],x,y);
    // printf("Tree size: %d\n",free_minimax_tree(node));
    printf("I played %d %d\n",*x,*y);
    int result = play_piece_if_legal(bd,*x,*y);
    if (!result) {
      printf("my move sucked...\n");
    }
    // show_othello_bd(stdout,bd);
    game_over = false;
  } else {
    printf("Player %d passes.\n",bd->turn);
    *x = *y = -1;
    // show_othello_bd(stdout,bd);
    game_over = pass_turn_is_game_over(bd);
  }
  if (!game_over) {
    trees[i] = cut_tree_for_move(trees[i],*x,*y);
    assert(boards_equal(trees[i]->bd,bd));
  }
  return game_over;
}

int main (int argc, char **argv) {
  struct timeval cur;
  gettimeofday(&cur,NULL);
  srand(time(NULL)); //cur.tv_usec);
  printf("%d\n",(int)cur.tv_usec);
  othello_bd *bd = new_othello_bd();
  init_learning_ai_game_state(&lags);
  show_othello_bd(stdout,bd);
  bool game_over = 0;
  int x,y;
  bool (*play[2]) (othello_bd*,int*,int*,int,int);
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
  } else {
    play[0] = play_human_turn_is_game_over;
    play[1] = play_minimax_ai_turn_is_game_over;
  }
  while (!game_over) {
    if (bd->turn == 1) {
      game_over = (play[0])(bd,&x,&y,x,y);
      add_board(&lags,bd);
      show_othello_bd(stdout,bd);
    } else if (bd->turn == -1) {
      game_over = (play[1])(bd,&x,&y,x,y);
      add_board(&lags,bd);
      show_othello_bd(stdout,bd);
    }
  }
  double result = score(bd);
  if (result > 0) {
    printf("Player 1 wins!\n");
  } else if (result < 0) {
    printf("Player -1 wins!\n");
  } else {
    printf("Tie!\n");
  }
  if (trees[0]) free_minimax_tree(trees[0]);
  if (trees[1]) free_minimax_tree(trees[1]);
  print_learning_ai_game_state(&lags,result ? result / abs(result) : 0);
  free ((void*)bd);
  exit(0);
}
