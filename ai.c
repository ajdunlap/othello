#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "board.h"
#include "play.h"
#include "show.h"
#include "math.h"
#include "ai.h"

// create a new minimax node, leaving the weight uninitialized
minimax_node *new_minimax_node (othello_bd *bd, int depth, double alpha, double beta) {
  minimax_node *node = (minimax_node*)malloc(sizeof(struct minimax_node));
  node->bd = new_othello_bd_copy(bd);
  node->depth = depth;
  node->alpha = alpha;
  node->beta = beta;
  node->has_weight = false;
  node->children = NULL;
  node->next_x = node->next_y = 0;
  return node;
}

// add a child to a minimax node
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

bool update_stats_from_child_should_prune (minimax_node *node, minimax_node *child_node);

// build a new child and return
// true if we should prune - i.e. stop processing the parent node
bool build_minimax_node_create_child_should_prune (double (*static_eval)(othello_bd*),minimax_node *node, int max_depth, othello_bd *new_bd, int i, int j) {
  minimax_node *child_node = new_minimax_node(new_bd,node->depth+1,node->alpha,node->beta);
  build_minimax_node_worker(static_eval,child_node,max_depth-1);
  bool prune = update_stats_from_child_should_prune (node, child_node);
  add_minimax_child(node,i,j,child_node);
  return prune;
}

// propagate alpha/beta values from child to parent and return true if we should
// prune - i.e. stop processing the parent node
bool update_stats_from_child_should_prune (minimax_node *node, minimax_node *child_node) {
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
  return node->alpha > node->beta;
}

bool build_minimax_node_process_child_should_prune (double (*static_eval)(othello_bd*),minimax_node *node, minimax_node *child, int max_depth);

// create a new node of the tree
void build_minimax_node_worker (double (*static_eval)(othello_bd*),minimax_node *node, int max_depth) {
  bool have_legal_moves = false;
  if (!max_depth) {
    node->weight = node->alpha = node->beta = static_eval(node->bd);
    assert(!isnan(node->weight));
    return;
  }
  minimax_node_c *child = node->children;
  while (child) {
    assert(node->next_x || node->next_y);
    have_legal_moves = true;
    if (build_minimax_node_process_child_should_prune (static_eval,node,child->node,max_depth)) {
      return;
    }
    child = child->next;
  }
  othello_bd new_bd;
  bool done = false;
  for ( ; !done && node->next_x < X_SIZE ; ++node->next_x) {
    for ( ; !done && node->next_y < Y_SIZE ; ++node->next_y) {
      copy_othello_bd(&new_bd,node->bd);
      if (play_piece_if_legal(&new_bd,node->next_x,node->next_y)) {
        have_legal_moves = true;
        if (build_minimax_node_create_child_should_prune (static_eval,node, max_depth, &new_bd,node->next_x,node->next_y)) {
          done = true;
        }
      }
    }
    node->next_y = 0;
  }
  if (!have_legal_moves) {
    copy_othello_bd(&new_bd,node->bd);
    new_bd.turn = -(new_bd.turn);
    build_minimax_node_create_child_should_prune (static_eval,node, max_depth, &new_bd,-1,-1);
  }
}

// recurse to a child we have already built (for a previous turn) and return
// true if we should prune - i.e. stop processing the parent node
bool build_minimax_node_process_child_should_prune (double (*static_eval)(othello_bd*),minimax_node *node, minimax_node *child, int max_depth) {
  child->alpha = node->alpha;
  child->beta = node->beta;
  build_minimax_node_worker(static_eval,child,max_depth-1);
  return update_stats_from_child_should_prune(node,child);
}

// build a minimax tree
minimax_node *build_minimax_tree (double (*static_eval)(othello_bd*),minimax_node *node, int max_nodes, othello_bd *bd, int max_depth) {
  if (!node) {
    node = new_minimax_node(bd,0,-INFINITY,INFINITY);
  }
  node->alpha = -INFINITY;
  node->beta = INFINITY;
  assert(boards_equal(bd,node->bd));
  build_minimax_node_worker(static_eval,node,max_depth);
  assert(node->children);
  return node;
}

minimax_node *cut_tree_for_move (minimax_node *node, int x, int y) {
  minimax_node_c *child = node->children;
  free((void*)node->bd);
  free((void*)node);
  minimax_node *new_node = NULL;
  //show_othello_bd(stdout,node->bd);
  if (child) {
    //printf("-x:%d,y:%d\n",x,y);
    //bool done = false;
    //printf("STARTING...%d %d\n",x,y);
    while (child) {
      minimax_node_c *current_child = child;
      if (child->move_x == x && child->move_y == y) {
        new_node = child->node;
        assert(child->node);
      } else {
        free_minimax_tree(child->node);
      }
      child = child->next;
      free((void*)current_child);
    }
    //assert(new_node);
  } /*else {
    //printf("xxx %d\n",node->bd->turn);
    assert(false);
    node = build_minimax_tree (node, 0, node->bd, 2);
    return cut_tree_for_move(node,x,y);
  }*/
  //free((void*)node->bd);
  //free((void*)node);
  return new_node;
  //free_minimax_tree(node);
  //return NULL;
}

// free memory for a minimax tree
int free_minimax_tree (minimax_node *node) {
  int nodes = 1;
  minimax_node_c *child = node->children;
  free((void*)node->bd);
  free((void*)node);
  while (child) {
    minimax_node_c *old_child = child;
    child = child->next;
    nodes += free_minimax_tree(old_child->node);
    free((void*)old_child);
  }
  return nodes;
}

// look at a minimax tree that has already been
// processed and get the best move for the first player
void best_move (minimax_node *node, int *x, int *y) {
  assert(have_legal_moves(node->bd));
  minimax_node_c *child = node->children;
  double maxrandsofar = node->bd->turn == 1 ? -INFINITY : INFINITY;
  assert(child);
  bool done = false;
  while (child) {
    double test = (rand() % 1000000) * child->node->weight;
    if (node->bd->turn == 1) {
      if (test >= maxrandsofar) {
        *x = child->move_x;
        *y = child->move_y;
        maxrandsofar = test;
        done = true;
      }
    } else {
      if (test <= maxrandsofar) {
        *x = child->move_x;
        *y = child->move_y;
        maxrandsofar = test;
        done = true;
      }
    }
    child = child->next;
  }
  assert(done);
}

void init_board_counts (board_counts *cts) {
  cts->turn = 0;
  cts->squares_filled = 0;
  cts->score = 0;
  for (int k = 0 ; k < NUM_SQUARE_CLASSES ; ++k) {
    cts->counts[k] = 0;
  }
}

board_counts compute_board_counts (othello_bd *bd) {
  board_counts cts;
  init_board_counts(&cts);
  for (int i = 0 ; i < X_SIZE ; ++i) {
    for (int j = 0 ; j < Y_SIZE ; ++j) {
      if (bd->board[i][j]) {
        cts.turn = bd->turn;
        ++cts.squares_filled;
        cts.score += bd->board[i][j];
        cts.counts[square_type(i,j)] += bd->board[i][j];
      }
    }
  }
  return cts;
}

// hand-tuned static evaluation functions
double hand_static_eval (othello_bd *bd) {
  board_counts cts = compute_board_counts(bd);
  if (cts.squares_filled < 64) {
    int result = 0;
    for (int k = 0 ; k < NUM_SQUARE_CLASSES ; ++k) {
      result += cts.counts[k]*type_hand_score(k);
    }
    return (double)result;
  } else {
    return cts.score ? INFINITY*((cts.score > 0) - (cts.score < 0)) : 0;
  }
}

// naive static evaluation function
double naive_static_eval (othello_bd *bd) {
  board_counts cts = compute_board_counts(bd);
  if (cts.squares_filled < 64) {
    int result = 0;
    for (int k = 0 ; k < NUM_SQUARE_CLASSES ; ++k) {
      result += cts.counts[k];
    }
    return (double)result;
  } else {
    return cts.score ? INFINITY*((cts.score > 0) - (cts.score < 0)) : 0;
  }
}

// show a minimax tree in graphviz format
// I mostly used this when debugging the minimax code, 
// so there may be some bitrot - it is not feasible for more
// than two or three plies
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
