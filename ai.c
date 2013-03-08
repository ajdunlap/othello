#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "board.h"
#include "play.h"
#include "show.h"
#include "math.h"
#include "ai.h"

minimax_node *new_minimax_node (othello_bd *bd, int depth, double alpha, double beta) {
  minimax_node *node = (minimax_node*)malloc(sizeof(struct minimax_node));
  node->bd = copy_othello_bd(bd);
  node->depth = depth;
  node->alpha = alpha;
  node->beta = beta;
  node->has_weight = false;
  node->children = NULL;
  // node->weight = static_eval(node->bd);
  return node;
}

void add_minimax_child (minimax_node *node, int move_x, int move_y, minimax_node *child_node) {
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

bool build_minimax_node_create_child_with_board (minimax_node *node, int max_depth, othello_bd *new_bd, int i, int j) {
  minimax_node *child_node = new_minimax_node(new_bd,node->depth+1,node->alpha,node->beta);
  build_minimax_node_worker(child_node,max_depth-1);
  if (node->bd->turn == 1) {
    node->alpha = fmax (node->alpha, child_node->beta);
    if (node->has_weight) {
      node->weight = fmax (node->weight, child_node->weight);
    } else {
      node->weight = child_node->weight;
      node->has_weight = true;
    }
  } else {
    node->beta = fmin (node->beta, child_node->alpha);
    if (node->has_weight) {
      node->weight = fmin (node->weight, child_node->weight);
    } else {
      node->weight = child_node->weight;
      node->has_weight = true;
    }
  }
  add_minimax_child(node,i,j,child_node);
  return node->alpha > node->beta;
}

void build_minimax_node_worker (minimax_node *node, int max_depth) {
  bool have_legal_moves = false;
  if (!max_depth) {
    node->weight = node->alpha = node->beta = static_eval(node->bd);
    return;
  }
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      othello_bd *new_bd = copy_othello_bd(node->bd);
      if (play_piece_if_legal(new_bd,i,j)) {
        have_legal_moves = true;
        if (build_minimax_node_create_child_with_board (node, max_depth, new_bd,i,j)) {
          // printf("!!!\n");
          return;
        }
      }
      free((void*)new_bd);
    }
  }
  if (!have_legal_moves) {
    othello_bd *new_bd = copy_othello_bd(node->bd);
    new_bd->turn = -(new_bd->turn);
    if (build_minimax_node_create_child_with_board (node, max_depth, new_bd,-1,-1)) {
      return;
    }
  }
}

minimax_node *build_minimax_tree (int max_nodes, othello_bd *bd, int max_depth) {
  minimax_node *node = new_minimax_node(bd,0,-INFINITY,INFINITY);
  build_minimax_node_worker(node,max_depth);
  return node;
}

void free_minimax_tree (minimax_node *node) {
  minimax_node_c *child = node->children;
  while (child) {
    minimax_node_c *old_child = child;
    child = child->next;
    free_minimax_tree(old_child->node);
    free((void*)old_child);
  }
  free((void*)node->bd);
  free((void*)node);
}

void best_move (minimax_node *node, int *x, int *y) {
  // eval_minimax_tree(node);
  // show_minimax_tree(node);
  minimax_node_c *child = node->children;
  int done = 0;
  while (child && !done) {
    if (child->node->weight == node->weight) {
      printf("--->%f,%d,%d\n",node->weight,child->move_x,child->move_y);
      *x = child->move_x;
      *y = child->move_y;
      done = 1;
    } else {
      child = child->next;
    }
  }
  if (!done) {
    printf("Don't wanna play...\n");
  }
}

void init_board_counts (board_counts *cts) {
  cts->score = 0;
  cts->squares_filled = 0;
  for (int k = 0 ; k < 3 ; ++k) {
    cts->counts[k] = 0;
  }
}

board_counts compute_board_counts (othello_bd *bd) {
  board_counts cts;
  init_board_counts(&cts);
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      if (bd->board[i][j]) {
        ++cts.squares_filled;
        cts.score += bd->board[i][j];
        cts.counts[distance_from_edge(i,j)] += bd->board[i][j];
      }
    }
  }
  return cts;
}

double static_eval (othello_bd *bd) {
  board_counts cts = compute_board_counts(bd);
  if (cts.squares_filled < 64) {
    int result = 0;
    for (int k = 0 ; k < 3 ; ++k) {
      result += cts.counts[k]*(4-k);
    }
    return (double)result;
  } else {
    return INFINITY*((cts.score > 0) - (cts.score < 0));
  }
}

void show_minimax_tree_worker (minimax_node *node) {
  // fprintf(stderr,"recursing - %d\n",node->depth);
  // fprintf(stderr,"digraph G {");
  fprintf(stderr,"%ld [label=\"%.0f<=%.0f<=%.0f\\n",(uint64_t)node,node->alpha,node->weight,node->beta); // Considering move %d %d for %d,weight: %f\n",child->move_x,child->move_y,node->bd->turn,child->node->weight);
  //show_othello_bd_for_dot(stderr,node->bd);
  fprintf(stderr,"\" style=filled fillcolor=%s];\n", -1 == node->bd->turn ? "cadetblue1" : "coral1");
  minimax_node_c *child = node->children;
  if (child) {
    do {
      for (int i = 0 ; i < node->depth ; ++i) {
        fprintf(stderr," ");
      }
      fprintf(stderr,"%ld -> %ld;\n",(uint64_t)node,(uint64_t)child->node); // Considering move %d %d for %d,weight: %f\n",child->move_x,child->move_y,node->bd->turn,child->node->weight);
      // show_othello_bd(stderr,child->node->bd);
      show_minimax_tree_worker(child->node);
    } while ((child = child->next));
  } else {
    for (int i = 0 ; i < node->depth ; ++i) {
      fprintf(stderr," ");
    }
    //fprintf(stderr,"Weight: %f\n",node->weight);
  }
}

void show_minimax_tree (minimax_node *node) {
  fprintf(stderr,"digraph G {\ngraph [fontname = \"courier\"];\nnode [fontname = \"courier\"];\nedge [fontname = \"courier\"];\n");
  show_minimax_tree_worker(node);
  fprintf(stderr,"}\n");
}
