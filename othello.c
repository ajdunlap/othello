#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "board.h"
#include "show.h"
#include "play.h"
#include "ai.h"
#include "random-ai.h"
#include "learning-ai.h"

#define MINIMAX_DEPTH_1 4
#define MINIMAX_DEPTH_2 3

minimax_node *trees[2] = { NULL, NULL };
learning_ai_weights weights;

double learning_static_eval (othello_bd *bd) {
  board_counts cts = compute_board_counts(bd);
  return weight_board_counts(&weights,&cts);
}

bool play_human_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y, int depth) {
  if (have_legal_moves(bd)) {
    while (true) {
      printf("Move for player %d? > ",bd->turn);
      if (scanf("%d %d",x,y) != 2) {
        while (getchar() != '\n'); // throw away this line
        printf("Bad move format; enter x y.\n");
      } else if (!play_piece_if_legal(bd,*x,*y)) {
        printf("Illegal move\n");
      } else {
        return 0;
      }
    }
  } else {
    printf("Move for player %d? > (PASS)\n",bd->turn);
    *x = *y = -1;
    return pass_turn_is_game_over(bd);
  }
}

bool play_random_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y, int depth) {
  // show_othello_bd(stdout,bd);
  random_move(bd,x,y);
  if (*x != -1) {
    // printf("I played %d %d\n",*x,*y);
    assert(play_piece_if_legal(bd,*x,*y));
    return false;
  } else {
    // printf("Player %d passes.\n",bd->turn);
    *x = *y = -1;
    return pass_turn_is_game_over(bd);
  }
}

bool play_minimax_ai_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y, int depth, double (*static_eval)(othello_bd*)) {
  bool game_over;
  int i = (1+bd->turn)>>1;
  if (trees[i]) {
    trees[i] = cut_tree_for_move(trees[i],opp_x,opp_y);
    assert(!trees[i] || boards_equal(trees[i]->bd,bd));
  }
  trees[i] = build_minimax_tree(static_eval,trees[i],0,bd,depth);
  if (have_legal_moves(bd)) {
    best_move(trees[i],x,y);
    int result = play_piece_if_legal(bd,*x,*y);
    assert(result);
    game_over = false;
  } else {
    *x = *y = -1;
    game_over = pass_turn_is_game_over(bd);
  }
  if (!game_over) {
    trees[i] = cut_tree_for_move(trees[i],*x,*y);
    assert(!trees[i] || boards_equal(trees[i]->bd,bd));
  }
  return game_over;
}

bool play_hand_minimax_ai_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y, int depth) {
  return play_minimax_ai_turn_is_game_over(bd, x, y, opp_x, opp_y, depth, static_eval);
}

bool play_learning_minimax_ai_turn_is_game_over(othello_bd *bd, int *x, int *y, int opp_x, int opp_y, int depth) {
  return play_minimax_ai_turn_is_game_over(bd, x, y, opp_x, opp_y, depth, learning_static_eval);
}

void usage () {
  fprintf(stderr,"Usage: othello [-n NGAMES] [-q] PLAYER1 PLAYER2\n"
                 "PLAYER1 and PLAYER2 can be any of human, cpu, learning-cpu, random\n"
                 "Options:\n"
                 "  -d [DEPTH|DEPTH1,DEPTH2]  : specify depth of minimax tree search\n"
                 "                              DEPTH can be either a single integer or two comma-separated integers\n"
                 "                              to specify the depth for each player\n"
                 "  -h                        : print this help\n"
                 "  -n NGAMES                 : plays n games\n"
                 "  -q                        : don't print out boards, just winners of each game (for cpu-vs-cpu play)\n");
}

int main (int argc, char **argv) {
  // initialize random number generator
  struct timeval cur;
  gettimeofday(&cur,NULL);
  srand(cur.tv_usec);

  // functions to play the game
  bool (*play[2]) (othello_bd*,int*,int*,int,int,int) = { NULL, NULL };

  // how many games should we play?
  int ngames = 1;

  // should we print out the boards?
  bool quiet = false;

  // minimax tree search depths
  int depth1 = 4, depth2 = 4;

  // parse command line opts
  int c;
  while (( c = getopt (argc, argv, "n:qhd:")) != -1) {
    char *tail;
    switch (c) {
      case 'n':
        ngames = strtol(optarg,&tail,10);
        if (*tail || ngames <= 0 ) {
          fprintf(stderr,"Option -n requires a positive integer argument\n");
          usage();
          exit(1);
        }
        break;
      case 'd':
        depth1 = strtol(optarg,&tail,10);
        if (!(*tail)) {
          depth2 = depth1;
        } else if (*tail == ',') {
          if (*(tail+1)) {
            depth2 = strtol(tail+1,&tail,10);
            if (*tail) {
              fprintf(stderr,"Option -d requires argument either DEPTH or DEPTH1,DEPTH2\n");
              usage();
              exit(1);
            }
          } else {
            fprintf(stderr,"Option -d requires argument either DEPTH or DEPTH1,DEPTH2\n");
            usage();
            exit(1);
          }
        } else {
          fprintf(stderr,"Option -d requires argument either DEPTH or DEPTH1,DEPTH2\n");
          usage();
          exit(1);
        }
        break;
      case 'q':
        quiet = true;
        break;
      case 'h':
        usage();
        exit(0);
        break;
      case '?':
        if (optopt == 'n') {
          fprintf(stderr,"Option -n requires an argument\n");
        }
        else if (isprint (optopt)) {
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        } else {
          fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        }
        usage();
        exit(1);
        break;
    }
  }

  bool have_human_player = false;

  for (int k = optind ; k < argc && k < 2 + optind ; ++k) {
    if (!strcmp(argv[k],"human")) {
      play[k-optind] = play_human_turn_is_game_over;
      have_human_player = true;
    } else if (!strcmp(argv[k],"cpu")) {
      play[k-optind] = play_hand_minimax_ai_turn_is_game_over;
    } else if (!strcmp(argv[k],"learning-cpu")) {
      play[k-optind] = play_learning_minimax_ai_turn_is_game_over;
    } else if (!strcmp(argv[k],"random")) {
      play[k-optind] = play_random_turn_is_game_over;
    } else {
      fprintf(stderr,"Player name must be 'human' or 'cpu'.");
      exit(1);
    }
  }

  if (!play[0]) {
    play[0] = play_human_turn_is_game_over;
    have_human_player = true;
  }

  if (!play[1]) {
    play[1] = play_hand_minimax_ai_turn_is_game_over;
  }


  if (quiet && have_human_player) {
    fprintf(stderr,"WARNING: Quiet mode enabled but a human is playing.\n"
                   "         This is going to make the game awfully hard for her or him!\n");
  }

  init_weights(&weights);

  int wins1 = 0, wins2 = 0, wins1backhalf = 0, wins2backhalf = 0;

  learning_ai_game_state *lags_arr = (learning_ai_game_state*)calloc(ngames,sizeof(learning_ai_game_state));
  for (int game = 0 ; game < ngames ; ++game) {
    othello_bd the_bd;
    othello_bd *bd = &the_bd;
    // initialize learning AI machinery in case a function ends up using it
    init_learning_ai_game_state(&lags_arr[game]);

    reset_othello_bd(bd);
    bool game_over = false;
    while (!game_over) {
      if (!quiet) {
        show_othello_bd(stdout,bd);
      }
      int x,y;
      game_over = (play[(1-bd->turn)>>1])(bd,&x,&y,x,y,bd->turn == 1 ? depth1 : depth2);
      if (!quiet) {
        // flip the turn here because the turn has already been changed
        if (x != -1) {
          printf("Player %d's move: %d %d.\n",-bd->turn,x,y);
        } else {
          printf("Player %d passes.\n",-bd->turn);
        }
      }
      add_board(&lags_arr[game],bd);
    }
    if (!quiet) {
      show_othello_bd(stdout,bd);
    }

    double winner = score(bd);

    if (winner > 0) {
      printf("Player 1 wins!\n");
      if (game > ngames/2) {
        ++wins1backhalf;
      }
      ++wins1;
    } else if (winner < 0) {
      printf("Player -1 wins!\n");
      ++wins2;
      if (game > ngames/2) {
        ++wins2backhalf;
      }
    } else {
      printf("Tie!\n");
    }
    for (int k = 0 ; k < 2 ; ++k) {
      if (trees[k]) {
        free_minimax_tree(trees[k]);
        trees[k] = NULL;
      }
    }
    // print_learning_ai_game_state(&lags_arr[game],winner ? winner / abs(winner) : 0);
    for (int l = 0 ; l < 10 ; ++l) {
      for (int k = game >= 0 ? game : 0 ; k <= game ; ++k) {
        update_weights_from_game(&weights,&lags_arr[k],winner,1);
      }
    }
    printf("WEIGHTS ");
    print_weights(&weights);
    printf("SCORE: 1: %d (%d); 2: %d (%d)\n",wins1,wins1backhalf,wins2,wins2backhalf);
  }

  exit(0);
}
