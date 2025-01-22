/**
 * \file
 * \brief .C file with realizations of tetris.h
 * 
 * \details This file contain all realizations
*/
#include "tetris.h"

int main(void) {
  game_loop();
  return 0;
}

/**
 * \brief main game loop
 * \details The function wich contain all game vatiables,
 * activate fsm and prinit all information
 * \version 1.0
 * \author vylarrca
 */
void game_loop() {
  board_t map;
  game_stats_t stats;
  tile *tiles;
  tile current_tile;
  tile next_tile;

  init_visual();
  reset_map(&map);
  stats_init(&stats);

  init_mem_tile_list(&tiles);
  tile_set(tiles);

  // init other variables
  int signal = 0;
  bool break_flag = true;
  game_state state = START;

  while (break_flag) {
    if (state == EXIT_STATE || state == FILE_ERROR_STATE) break_flag = false;

    if (state == MOVING) {
      stats.cur_time = get_time();
      if (stats.cur_time > stats.shift_time) {
        state = SHIFTING;
        stats.shift_time = stats.cur_time + SHIFT_TIME / (stats.level + 1);
      }
    }

    sigact(get_signal(signal), &state, &stats, &map, &current_tile, &next_tile,
           tiles);

    ////////////// print ////////////////
    if (state != EXIT_STATE && state != GAMEOVER && state != FILE_ERROR_STATE &&
        state != PAUSE) {
      params_t params = compress_params(&state, &stats, &map, &current_tile,
                                        &next_tile, tiles);
      upd_borad(params);
    }
    ///////////////////////////////////////

    if (state == MOVING || state == START || state == GAMEOVER ||
        state == PAUSE)
      signal = getch();
  }

  tile_destroy(tiles);
  endwin();
}
