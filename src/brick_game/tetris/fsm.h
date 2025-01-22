#ifndef C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_FSM_H_
#define C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_FSM_H_

#include "../../gui/cli/frontend.h"
#include "backend.h"

void sigact(signals sig, game_state *state, game_stats_t *stats, board_t *map,
            tile *current, tile *next, tile *tiles);
signals get_signal(int user_input);

#endif  // C7_BRICKGAME_V1_0_1_SRC_BRICK_GAME_TETRIS_FSM_H_