#ifndef C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_BACKEND_H_
#define C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_BACKEND_H_

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SHIFT_TIME 1000
#define WAIT_GET 4

#define YOU_LOSE_SCR "./gui/cli/game_over.txt"
#define PAUSE_SCR "./gui/cli/pause.txt"
#define NEW_LVL_SCR "./gui/cli/new_lvl.txt"

// board suppose to be bigger than map
#define BOARD_H 68
#define BOARD_V 24
#define MAP_H 10
#define MAP_V 20
#define TILE_SIZE 4
#define TILE_AMOUNT 7

#define BOARD_CHAR '.'
#define TILE_CHAR '#'
#define SPACE_CHAR 32

// #define KEY_UP 'w'
#define KEY_D 115
#define KEY_L 97
#define KEY_R 100
#define KEY_S 32
#define KEY_ESCAPE 27

#define GET_USER_INPUT getch()

typedef struct backend {
  int map_ar[MAP_V][MAP_H];
} board_t;

typedef struct tile_array {
  int x, y;
  int size_x, size_y;
  int container[TILE_SIZE][TILE_SIZE];
} tile;

typedef struct statistics {
  int best_score;
  int score;
  int level;
  long int cur_time;
  long int shift_time;
} game_stats_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  PAUSE,
  FILE_ERROR_STATE
} game_state;

typedef enum {
  MOVE_SLOW = 0,
  MOVE_FAST,
  MOVE_RIGHT,
  MOVE_LEFT,
  ROTATE_BTN,
  ESCAPE_BTN,
  NOSIG
} signals;

typedef struct game_params {
  game_stats_t *stats;
  game_state *state;
  board_t *map;
  tile *tile;
  tile *next_tile;
  tile *all_tiles;
} params_t;

void tile_rotate(tile *self);
void tile_copymap(tile *dst, tile src);
void tile_copy(tile *dst, tile src);
void tile_zero(tile *self);
void tile_set(tile *tile_list);
void init_mem_tile_list(tile **tile_list);
tile get_rand_tile(tile *tile_list);
void tile_destroy(tile *tile_list);

void stats_init(game_stats_t *stats);
void load_stats(game_stats_t *stats);
void save_stats(game_stats_t stats);
long get_time();
params_t compress_params(game_state *state, game_stats_t *stats, board_t *map,
                         tile *current, tile *next, tile *tiles);

#endif  // C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_BACKEND_H_
