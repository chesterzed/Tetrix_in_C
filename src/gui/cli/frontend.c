/**
 * \file
 * \brief .C file with realizations of frontend.h
 * \details This file contain all realizations
*/
//
// Created by ChesterZed on 07.05.2024.
//
#include "frontend.h"

char board[BOARD_V][BOARD_H];
/**
 * \brief Set ncurses's preferences
 * \details Wait time depends on WAIT_GET const measure in 0.1 sec (1 integer is 0.1 sec)
 * \version 1.0
 * \author vylarrca
*/
void init_visual() {
  initscr();
  echo();
  curs_set(0);
  keypad(stdscr, FALSE);

  // 1 integer is 0.1 sec
  // SHIFT_TIME MEASURE IN MILLISECONDS
  halfdelay(WAIT_GET);
}

/**
 * \brief Clear, compile and print
 * \details Nothing interesting just ncurses and board[][]
 * \version 1.0
 * \author vylarrca
*/
void upd_borad(params_t prms) {
  clear();
  fill_board();
  add_map(prms);
  add_next_tile(*prms.next_tile);
  add_score(prms);
  print_board();
  refresh();
}

/**
 * \brief Fill all map by SPACE_CHAR
 * \details Soo...
 * \version 1.0
 * \author vylarrca
*/
void reset_map(board_t *map) {
  for (int i = 0; i < MAP_V; ++i) {
    for (int j = 0; j < MAP_H; ++j) {
      map->map_ar[i][j] = SPACE_CHAR;
    }
  }
}

/**
 * \brief Background filling
 * \details Fill all map by BOARD_CHAR
 * \version 1.0
 * \author vylarrca
*/
void fill_board() {
  for (int i = 0; i < BOARD_V; ++i) {
    for (int j = 0; j < BOARD_H; ++j) {
      board[i][j] = BOARD_CHAR;
    }
  }
}

/**
 * \brief Add score, bestscore and level to board
 * \details Make score 'print ready' xd. Just add it to the board[][]
 * \version 1.0
 * \author vylarrca
*/
void add_score(params_t prms) {
  int x = BOARD_H / 2 + MAP_H / 2 + TILE_SIZE;
  int y = BOARD_V / 2 + MAP_V / 2 - 3 * TILE_SIZE - 3;
  for (int i = 0; i < TILE_SIZE + 2; ++i) {
    for (int j = 0; j < TILE_SIZE + 8; ++j) {
      board[y + i][x + j - 1] = ' ';
    }
  }

  char c[] = "Level:";
  strcpy(&board[y][x], c);
  board[y][x + strlen(c)] = ' ';

  char dig[11];
  sprintf(dig, "%d", prms.stats->level);
  strcpy(&board[y + 1][x], dig);
  board[y + 1][x + strlen(dig)] = ' ';

  char c1[] = "Best Score:";
  strcpy(&board[y + 2][x], c1);
  board[y + 2][x + strlen(c1)] = ' ';

  char dig1[11];
  sprintf(dig1, "%d", prms.stats->best_score);
  strcpy(&board[y + 3][x], dig1);
  board[y + 3][x + strlen(dig1)] = ' ';

  char c2[] = "Score:";
  strcpy(&board[y + 4][x], c2);
  board[y + 4][x + strlen(c2)] = ' ';

  char dig2[11];
  sprintf(dig2, "%d", prms.stats->score);
  strcpy(&board[y + 5][x], dig2);
  board[y + 5][x + strlen(dig2)] = ' ';

  add_box(y - 1, x - 1, y + 6, x + 11);
}

/**
 * \brief Add map to board
 * \details Make map 'print ready' xd. Just add it to the center of board[][]
 * \version 1.0
 * \author vylarrca
*/
void add_map(params_t prms) {
  int map_x = BOARD_H / 2 - MAP_H / 2;
  int map_y = BOARD_V / 2 - MAP_V / 2;
  for (int i = 0; i < MAP_V; ++i) {
    for (int j = 0; j < MAP_H; ++j) {
      board[i + map_y][j + map_x] = prms.map->map_ar[i][j];
    }
  }
  add_box(map_y - 1, map_x - 1, map_y + MAP_V, map_x + MAP_H);
  add_tile(*prms.tile, map_x + prms.tile->x, map_y + prms.tile->y);
}

/**
 * \brief Add tile to the board by coords x y
 * \details board[y][x]
 * \version 1.0
 * \author vylarrca
*/
void add_tile(tile tile, int x, int y) {
  for (int i = 0; i < TILE_SIZE; ++i) {
    for (int j = 0; j < TILE_SIZE; ++j) {
      if (tile.container[i][j] == TILE_CHAR)
        board[i + y][j + x] = tile.container[i][j];
    }
  }
}
/**
 * \brief Add next tile to the board
 * \details Uses add_tile(tile tile, int x, int y) to add a nexttile to right side of the map
 * \version 1.0
 * \author vylarrca
*/
void add_next_tile(tile next_tile) {
  int x = BOARD_H / 2 + MAP_H / 2 + TILE_SIZE;
  int y = BOARD_V / 2 + MAP_V / 2 - TILE_SIZE;
  for (int i = 0; i < TILE_SIZE; ++i) {
    for (int j = 0; j < TILE_SIZE + 3; ++j) {
      board[y + i - 1][x + j - 1] = ' ';
    }
  }
  add_box(y - 1, x - 1, y + TILE_SIZE - 1, x + TILE_SIZE + 2);
  add_tile(next_tile, x + 1, y);
}

/**
 * \brief Make the board visible
 * \details Uses ncurses and double loop for print the board[][]. \n P.S. (no refresh() inside)
 * \version 1.0
 * \author vylarrca
*/
void print_board() {
  for (int i = 0; i < BOARD_V; ++i) {
    for (int j = 0; j < BOARD_H; ++j) {
      printw("%c", board[i][j]);
    }
    printw("\n");
  }
}

/**
 * \brief Make corners to the board
 * \details You can assume that these are the coordinates of the upper left and lower right corners
 * int ty - top y
 * int lx - left x
 * int by - bottom y
 * int rx - right x
 * \version 1.0
 * \author vylarrca
*/
void add_box(int ty, int lx, int by, int rx) {
  for (int i = lx + 1; i < rx; ++i) {
    board[ty][i] = '~';
    board[by][i] = '~';
  }
  for (int i = ty + 1; i < by; ++i) {
    board[i][lx] = '|';
    board[i][rx] = '|';
  }
  board[ty][lx] = '#';
  board[ty][rx] = '#';
  board[by][lx] = '#';
  board[by][rx] = '#';
}

/**
 * \brief Print pause screen
 * \details Get 'Pause' from file
 * \version 1.0
 * \author vylarrca
*/
void print_pause() {
  clear();
  banner_t banner;
  reset_banner(&banner);
  // suppose to write ...(!read....
  if (read_banner(PAUSE, &banner)) {
    add_box(0, 0, BOARD_V - 1, BOARD_H - 1);
    for (int i = 0; i < BOARD_V - 2; ++i) {
      for (int j = 0; j < BOARD_H - 2; ++j) {
        board[i + 1][j + 1] = (int)banner.matrix[i][j];
      }
    }
  }
  print_board();
  refresh();
}

/**
 * \brief Print game over screen
 * \details Get 'game over' from file
 * \version 1.0
 * \author vylarrca
*/
void print_game_over() {
  clear();
  banner_t banner;
  // suppose to write ...(!read....
  if (read_banner(GAMEOVER, &banner)) {
    add_box(0, 0, BOARD_V - 1, BOARD_H - 1);
    for (int i = 0; i < BOARD_V - 2; ++i) {
      for (int j = 0; j < BOARD_H - 2; ++j) {
        board[i + 1][j + 1] = (int)banner.matrix[i][j];
      }
    }
  }
  print_board();
  refresh();
}

/**
 * \brief Fill all banner by SPACE_CHAR
 * \details Soo... Double loop
 * \version 1.0
 * \author vylarrca
*/
void reset_banner(banner_t *banner) {
  for (int i = 0; i < BOARD_V - 2; ++i) {
    for (int j = 0; j < BOARD_H - 2; ++j) {
      banner->matrix[i][j] = SPACE_CHAR;
    }
  }
}

/**
 * \brief Read a banner
 * \details As the first argument can get only 2 statesbanner_t GAMEOVER and PAUSE. And set it as a second argument
 * *banner gets a pointer to variable type banner_t when the funncion finish, this pointer will have a banner
 * Function returns 0 - SUCCES and 1 - FAIL
 * \version 1.0
 * \author vylarrca
*/
int read_banner(const game_state state, banner_t *banner) {
  int rc = 0;
  reset_banner(banner);
  FILE *file = NULL;
  if (state == GAMEOVER)
    file = fopen(YOU_LOSE_SCR, "r");
  else if (state == PAUSE)
    file = fopen(PAUSE_SCR, "r");
  else if (state == ATTACHING)
    file = fopen(NEW_LVL_SCR, "r");

  if (file) {
    board[2][1] = '#';

    for (int i = 0; i < BOARD_V - 2 && !rc; i++) {
      if (fgets(banner->matrix[i], BOARD_H - 5, file) == NULL)
        rc = 1;
      else
        banner->matrix[i][strcspn(banner->matrix[i], "\n")] = '\0';
    }
    fclose(file);
  } else {
    rc = 1;
  }

  return rc;
}
