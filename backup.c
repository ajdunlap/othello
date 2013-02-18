#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// x is rows
// y is columns

#define X_SIZE 8
#define Y_SIZE 8

typedef struct othello_bd {
  char board[X_SIZE][Y_SIZE];
  char turn;
} othello_bd;

othello_bd *new_othello_bd (void) {
  othello_bd *bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  memset(bd->board,0,X_SIZE*Y_SIZE*sizeof(char));
  bd->board[3][3] = bd->board[4][4] = 1;
  bd->board[3][4] = bd->board[4][3] = -1;
  bd->turn = 1;
  return bd;
}

othello_bd *copy_othello_bd (othello_bd *old_bd) {
  othello_bd *new_bd = (othello_bd*)malloc(sizeof(struct othello_bd));
  memcpy(new_bd,old_bd,sizeof(struct othello_bd));
  return new_bd;
}

void free_othello_bd (othello_bd *bd) {
  free((void*)bd);
}

void show_othello_piece(char c) {
  switch (c) {
    case -1:
      printf("X");
      break;
    case 0:
      printf(" ");
      break;
    case 1:
      printf("O");
      break;
    default:
      break;
  }
}

void print_horiz_border (void) {
  printf(" +");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    printf("-");
  }
  printf("+\n");
}

void show_othello_bd (othello_bd *bd) {
  printf("  ");
  for (int j = 0 ; j < X_SIZE ; ++j) {
    printf("%d",j);
  }
  printf("\n");
  print_horiz_border();
  for (int i = 0 ; i < Y_SIZE ; ++i) {
    printf("%d|",i);
    for (int j = 0 ; j < X_SIZE ; ++j) {
      show_othello_piece(bd->board[i][j]);
    }
    printf("|\n");
  }
  print_horiz_border();
}

int play_piece_or_legal_move (int play,othello_bd *bd,int x,int y) {
  int legal_move = 0;
  for (int x_dir = -1 ; x_dir <= 1 ; ++x_dir) {
    for (int y_dir = -1 ; y_dir <= 1 ; y_dir += (x_dir ? 1 : 2)) { // skip 0 if x_dir is 0
      char max_x = x;
      char max_y = y;
      char on_board = 1;
      do {
        max_x += x_dir;
        max_y += y_dir;
      } while ((on_board = (max_x >= 0 && max_y >= 0 && max_x < X_SIZE && max_y < Y_SIZE)) && bd->board[max_x][max_y] == -bd->turn);
      if (on_board && bd->board[max_x][max_y] == bd->turn) {
        int cur_x = x+x_dir;
        int cur_y = y+y_dir;
        printf("start(%d,%d),max(%d,%d),cur(%d,%d),dir(%d,%d),ok(%d)\n",x,y,max_x,max_y,cur_x,cur_y,x_dir,y_dir,cur_x*x_dir < max_x*x_dir && cur_y*y_dir < max_y*y_dir);
        for ( ; cur_x*x_dir < max_x*x_dir || cur_y*y_dir < max_y*y_dir ; cur_x += x_dir, cur_y += y_dir) {
          printf("FO");
          if (play) {
            legal_move = 1;
            bd->board[cur_x][cur_y] = bd->turn;
          } else {
            return 1;
          }
        }
      }
    }
  }
  if (play && legal_move) {
    bd->board[x][y] = bd->turn;
    bd->turn = -(bd->turn);
  }
  return legal_move;
}

int play_piece_if_legal (othello_bd *bd,int x,int y) {
  return play_piece_or_legal_move (1,bd,x,y);
}

int legal_move (othello_bd *bd,int x,int y) {
  return play_piece_or_legal_move(1,bd,x,y);
}

int main () {
  othello_bd *bd = new_othello_bd();
  while (1) {
    show_othello_bd(bd);
    printf("Move for player %d? > ",bd->turn);
    int x,y;
    if (scanf("%d %d",&x,&y) != 2) {
      while (getchar() != '\n'); // throw away this line
      printf("Bad move format; enter x y.\n");
      continue;
    } else if (!play_piece_if_legal(bd,x,y)) {
      printf("Illegal move\n");
    }
  }
}
