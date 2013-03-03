//#ifndef FRINGE_H
//#define FRINGE_H
typedef struct minimax_node minimax_node;

typedef struct fringe {
    int max_size, cur_size;
    minimax_node **arr;
} fringe;

fringe *new_fringe (minimax_node *top);

void free_fringe (fringe *fh);

void expand_fringe (fringe *fh);

minimax_node *pop_fringe (fringe *fh);

void insert_fringe (fringe *fh, minimax_node *node);
// #endif
