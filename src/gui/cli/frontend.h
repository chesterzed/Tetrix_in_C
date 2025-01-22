//
// Created by ChesterZed on 07.05.2024.
//

#ifndef C7_BRICKGAME_V1_0_1_SRC_GUI_CLI_FRONTEND_H_
#define C7_BRICKGAME_V1_0_1_SRC_GUI_CLI_FRONTEND_H_

#include "../../brick_game/tetris/fsm.h"
#include "../../brick_game/tetris/backend.h"

extern char board[BOARD_V][BOARD_H];

typedef struct {
  char matrix[BOARD_V - 2][BOARD_H - 2];
} banner_t;

void init_visual();
void upd_borad(params_t prms);
void reset_map(board_t *map);
void fill_board();
void add_score(params_t prms);
void add_map(params_t prms);
void add_tile(tile tile, int x, int y);
void add_next_tile(tile next_tile);
void print_board();

void add_box(int ty, int lx, int by, int rx);

void print_pause();
void print_game_over();
void reset_banner(banner_t *banner);

int read_banner(const game_state state, banner_t *banner);

#endif  // C7_BRICKGAME_V1_0_1_SRC_GUI_CLI_FRONTEND_H_
