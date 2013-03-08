#ifndef AI_H
#define AI_H
#include <stdbool.h>
#include "board.h"
typedef struct minimax_node {
  othello_bd *bd;
  struct minimax_node_c *children;
  double weight, alpha, beta;
  bool has_weight;
  int depth;
} minimax_node;

typedef struct minimax_node_c {
  int move_x,move_y;
  minimax_node *node;
  struct minimax_node_c *next;
} minimax_node_c;

minimax_node *new_minimax_node (othello_bd *bd,int depth,double alpha,double beta);
void free_minimax_tree (minimax_node *node);

void add_minimax_child (minimax_node *node, int move_x, int move_y, minimax_node *child_node);

minimax_node *build_minimax_tree (int max_nodes, othello_bd *bd, int max_depth);

void eval_minimax_tree (minimax_node *node);

void best_move (minimax_node *node, int *x, int *y);

double static_eval (othello_bd *bd);
void show_minimax_tree (minimax_node *node);
bool build_minimax_node_create_child_with_board (minimax_node *node, int max_depth, othello_bd *new_bd, int i, int j);
void build_minimax_node_worker (minimax_node *node, int max_depth);

#endif
