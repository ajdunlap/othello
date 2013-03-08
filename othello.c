#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "show.h"
#include "play.h"
#include "ai.h"

bool play_human_turn_is_game_over(othello_bd *bd) {
  if (have_legal_moves(bd)) {
    while (true) {
      printf("Move for player %d? > ",bd->turn);
      int x,y;
      if (scanf("%d %d",&x,&y) != 2) {
        while (getchar() != '\n'); // throw away this line
        printf("Bad move format; enter x y.\n");
      } else if (!play_piece_if_legal(bd,x,y)) {
        printf("Illegal move\n");
      } else {
        return 0;
      }
    }
  } else {
    printf("Player %d passes.\n",bd->turn);
    return pass_turn_is_game_over(bd);
  }
}

bool play_ai_turn_is_game_over(othello_bd *bd) {
  if (have_legal_moves(bd)) {
    show_othello_bd(stdout,bd);
    printf("Static evaluation: %f\n",static_eval(bd));
    minimax_node *node = build_minimax_tree(0,bd,6);
    //show_minimax_tree(node);
    int x;
    int y;
    best_move(node,&x,&y);
    free_minimax_tree(node);
    printf("I played %d %d\n",x,y);
    int result = play_piece_if_legal(bd,x,y);
    if (!result) {
      printf("my move sucked...\n");
    }
    show_othello_bd(stdout,bd);
    return 0;
  } else {
    printf("Player %d passes.\n",bd->turn);
    show_othello_bd(stdout,bd);
    return pass_turn_is_game_over(bd);
  }
}

int main () {
  othello_bd *bd = new_othello_bd();
  show_othello_bd(stdout,bd);
  bool game_over = 0;
  while (!game_over) {
    if (bd->turn == 1) {
      game_over = play_human_turn_is_game_over(bd);
    } else if (bd->turn == -1) {
      game_over = play_ai_turn_is_game_over(bd);
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
  free ((void*)bd);
  exit(0);
}
