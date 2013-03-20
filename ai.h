#ifndef AI_H
#define AI_H
#include <stdbool.h>
#include "board.h"
typedef struct board_counts {
  int turn;                       // whose turn it is
  int squares_filled;             // total number of pieces on the board
  int score;                      // current score
  int counts[NUM_SQUARE_CLASSES]; // array from square class to score of squares in that class
} board_counts;

typedef struct minimax_node {
  othello_bd *bd;
  struct minimax_node_c *children;
  double weight, alpha, beta;
  bool has_weight;    // has a weight been assigned to this square?
  int depth;          // how deep are we in the tree?
  int next_x,next_y;  // because of alpha-beta pruning, we often stop traversing a node before we look at
                      // all of the moves - but since we re-use the tree, the next turn we may need to look at more moves
                      // so these values tell us which moves we left off looking at so we can just build new child nodes
                      // for nodes we haven't already explored
} minimax_node;

// minimax node children, stored as a linked list
typedef struct minimax_node_c {
  int move_x,move_y;
  minimax_node *node;
  struct minimax_node_c *next;
} minimax_node_c;

minimax_node *new_minimax_node (othello_bd *bd,int depth,double alpha,double beta);
int free_minimax_tree (minimax_node *node);

void add_minimax_child (minimax_node *node, int move_x, int move_y, minimax_node *child_node);

minimax_node *build_minimax_tree (double (*static_eval) (othello_bd*),minimax_node *node,int max_nodes, othello_bd *bd, int max_depth);

void eval_minimax_tree (minimax_node *node);

void best_move (minimax_node *node, int *x, int *y);

board_counts compute_board_counts (othello_bd *bd);
double hand_static_eval (othello_bd *bd);
double naive_static_eval (othello_bd *bd);
void show_minimax_tree (minimax_node *node);
bool build_minimax_node_create_child_with_board (double (*static_eval) (othello_bd*), minimax_node *node, int max_depth, othello_bd *new_bd, int i, int j);
void build_minimax_node_worker (double (*static_eval) (othello_bd*), minimax_node *node, int max_depth);

minimax_node *cut_tree_for_move(minimax_node*,int,int);

#endif
