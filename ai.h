#ifndef AI_H
#define AI_H
#include "board.h"
typedef struct minimax_node {
  othello_bd *bd;
  struct minimax_node_c *children;
  double weight, alpha, beta;
  int depth;
} minimax_node;

typedef struct minimax_node_c {
  int move_x,move_y;
  minimax_node *node;
  struct minimax_node_c *next;
} minimax_node_c;

minimax_node *new_minimax_node (othello_bd *bd,int depth);
void free_minimax_tree (minimax_node *node);

minimax_node *add_minimax_child (minimax_node *node, int move_x, int move_y, minimax_node *child_node);

minimax_node *build_minimax_tree (int max_nodes, othello_bd *bd, int max_depth);

void eval_minimax_tree (minimax_node *node);

int best_move (minimax_node *node, int *x, int *y);

double static_eval (othello_bd *bd);
void show_minimax_tree (minimax_node *node);
#endif
