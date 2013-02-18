#include <stdio.h>
#include "board.h"
#include "show.h"
#include "play.h"
#include "ai.h"

int main () {
  othello_bd *bd = new_othello_bd();
  show_othello_bd(bd);
  while (1) {
    printf("Move for player %d? > ",bd->turn);
    int x,y;
    if (scanf("%d %d",&x,&y) != 2) {
      while (getchar() != '\n'); // throw away this line
      printf("Bad move format; enter x y.\n");
      continue;
    } else if (!play_piece_if_legal(bd,x,y)) {
      printf("Illegal move\n");
    } else {
      minimax_node *node = build_minimax_tree(8,1,bd);
      show_othello_bd(bd);
      int x;
      int y;
      best_move(node,&x,&y);
      printf("I played... %d %d\n",x,y);
      int result = play_piece_if_legal(bd,x,y);
      if (!result) {
        printf("my move sucked...\n");
      }
      show_othello_bd(bd);
    }
  }
}
