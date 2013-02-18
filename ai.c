#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "play.h"
#include "math.h"
#include "ai.h"

minimax_node *new_minimax_node (othello_bd *bd) {
  minimax_node *node = (minimax_node*)malloc(sizeof(struct minimax_node));
  node->bd = copy_othello_bd(bd);
  node->children = NULL;
  return node;
}

minimax_node *add_minimax_child (minimax_node *node, int move_x, int move_y, minimax_node *child_node) {
  minimax_node_c *current_child = node->children;
  minimax_node_c *child = (minimax_node_c*)malloc(sizeof(struct minimax_node_c));
  child->move_x = move_x;
  child->move_y = move_y;
  child->node = child_node;
  child->next = NULL;
  if (current_child) {
    while (current_child->next) {
      current_child = current_child->next;
    }
    current_child->next = child;
  } else {
    node->children = child;
  }
}

minimax_node *build_minimax_tree (int max_depth,int depth, othello_bd *bd) {
  minimax_node *node = new_minimax_node(bd);
  if (depth < max_depth) {
    for (int i = 0 ; i < X_SIZE ; ++i) {
      for (int j = 0 ; j < Y_SIZE ; ++j) {
        othello_bd *new_bd = copy_othello_bd(bd);
        if (play_piece_if_legal(new_bd,i,j)) {
          minimax_node *child_node = build_minimax_tree(max_depth,depth+1,bd);
          add_minimax_child(node,i,j,child_node);
        }
      }
    }
  }
  return node;
}

void eval_minimax_tree (minimax_node *node) {
  minimax_node_c *child = node->children;
  if (child) {
    node->weight = 0;
    do {
      eval_minimax_tree(child->node);
      if (node->bd->turn == 1) {
        node->weight = fmin(node->weight,child->node->weight);
      } else {
        node->weight = fmax(node->weight,child->node->weight);
      }
    } while (child = child->next);
  } else {
    node->weight = static_eval(node->bd);
  }
}

int best_move (minimax_node *node, int *x, int *y) {
  eval_minimax_tree(node);
  minimax_node_c *child = node->children;
  int done = 0;
  while (child && !done) {
    if (child->node->weight == node->weight) {
      printf("--->%d,%d\n",child->move_x,child->move_y);
      *x = child->move_x;
      *y = child->move_y;
      done = 1;
    } else {
      child = child->next;
    }
  }
}

double static_eval (othello_bd *bd) {
  double result = 0;
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      result += bd->board[i][j];
    }
  }
  return result;
}
