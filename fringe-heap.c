#include <stdbool.h>
#include "fringe-heap.h"
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

fringe_heap *new_fringe_heap (minimax_node *top) {
  fringe_heap *fh = (fringe_heap*)malloc(sizeof(struct fringe_heap));
  fh->max_size = fh->cur_size = 1;
  fh->heap = (minimax_node**)calloc(fh->max_size,sizeof(minimax_node*));
  fh->heap[0] = top;
  return fh;
}

void expand_fringe_heap (fringe_heap *fh) {
  printf("expanding...\n");
  fh->max_size *= 2;
  fh->heap = (minimax_node**)realloc(fh->heap,fh->max_size*sizeof(minimax_node*));
  if (!fh->heap) {
    assert(false);
  }
}

int heap_index (int level, int posn) {
  return (1 << level) + posn;
}

int heap_level (int i) {
  int level = 0;
  if (i == 0) {
    return 0;
  } else {
    int j = i+1;
    while (j >>= 1) {
      ++level;
    }
    return level;
  }
}

int heap_posn (int i, int level) {
  return level ? (i+1)%(1 << level) : 0;
}

int heap_left_child (int i) {
  int level = heap_level(i);
  return heap_index(level+1,2*heap_posn(i,level));
}

int heap_right_child (int i) {
  int level = heap_level(i);
  return heap_index(level+1,2*heap_posn(i,level)+1);
}

void swap_elts (fringe_heap *fh, int i1, int i2) {
  minimax_node *tmp = fh->heap[i1];
  fh->heap[i1] = fh->heap[i2];
  fh->heap[i2] = tmp;
}

void free_fringe_heap (fringe_heap *fh) {
  free((void*)fh->heap);
  free((void*)fh);
}

void heapify (fringe_heap *fh) {
  int i = 0;
  while (true) {
    int l = heap_left_child(i);
    int r = heap_right_child(i);
    minimax_node *ln = l < fh->cur_size ? fh->heap[l] : NULL;
    minimax_node *rn = r < fh->cur_size ? fh->heap[r] : NULL;
    if (l < fh->cur_size) {
      int to_swap;
      if (r < fh->cur_size) {
        to_swap = fh->heap[l]->weight < fh->heap[r]->weight ? l : r;
      } else {
        to_swap = l;
      }
      swap_elts(fh,i,to_swap);
      i = to_swap;
    } else {
      break;
    }
  }
}

minimax_node *pop_min_fringe_heap (fringe_heap *fh) {
  if (fh->cur_size) {
    minimax_node *node = fh->heap[0];
    fh->heap[0] = fh->heap[fh->cur_size-1];
    --fh->cur_size;
    heapify(fh);
    return node;
  } else {
    return NULL;
  }
}

void insert_fringe_heap (fringe_heap *fh, minimax_node *node) {
  ++fh->cur_size;
  while (fh->cur_size > fh->max_size) {
    expand_fringe_heap(fh);
  }
  fh->heap[fh->cur_size-1] = node;
  heapify(fh);
}
