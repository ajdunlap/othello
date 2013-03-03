#include <stdbool.h>
#include "fringe.h"
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

fringe *new_fringe (minimax_node *top) {
  fringe *fh = (fringe*)malloc(sizeof(struct fringe));
  fh->max_size = fh->cur_size = 1;
  fh->arr = (minimax_node**)calloc(fh->max_size,sizeof(minimax_node*));
  fh->arr[0] = top;
  return fh;
}

void free_fringe (fringe *fh) {
  free((void*)fh->arr);
  free((void*)fh);
}

void expand_fringe (fringe *fh) {
  fh->max_size *= 2;
  fh->arr = (minimax_node**)realloc(fh->arr,fh->max_size*sizeof(minimax_node*));
  if (!fh->arr) {
    assert(false);
  }
}

minimax_node *pop_fringe (fringe *fh) {
  if (fh->cur_size) {
    minimax_node *node = fh->arr[fh->cur_size-1];
    --fh->cur_size;
    return node;
  } else {
    return NULL;
  }
}

void insert_fringe (fringe *fh, minimax_node *node) {
  ++fh->cur_size;
  while (fh->cur_size > fh->max_size) {
    expand_fringe(fh);
  }
  fh->arr[fh->cur_size-1] = node;
}
