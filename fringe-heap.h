#include <stdlib.h>
typedef struct minimax_node minimax_node;

typedef struct fringe_heap {
  int max_size, cur_size;
  minimax_node **heap;
} fringe_heap;

fringe_heap *new_fringe_heap (minimax_node *top);

void expand_fringe_heap (fringe_heap *fh);

int heap_index (int level, int posn);

int heap_level (int i);

int heap_posn (int i, int level);

int heap_left_child (int i);

int heap_right_child (int i);

void swap_elts (fringe_heap *fh, int i1, int i2);

void free_fringe_heap (fringe_heap *fh);

void heapify (fringe_heap *fh);

minimax_node *pop_min_fringe_heap (fringe_heap *fh);

void insert_fringe_heap (fringe_heap *fh, minimax_node *node);
