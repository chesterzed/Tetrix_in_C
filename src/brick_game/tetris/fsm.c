/**
 * \file
 * \brief .C file with realizations of fsm.h
 * \details This file contain all realizations
*/
#include "fsm.h"

typedef void (*action)(params_t *prms);

void start_g(params_t *prms);
void spawn(params_t *prms);
bool check_collision(params_t *prms);
bool check_falling(params_t *prms);
void shift(params_t *prms);
void attach(params_t *prms);

void move_down(params_t *prms);
void move_right(params_t *prms);
void move_left(params_t *prms);
void rotate(params_t *prms);

void gam_ov(params_t *prms);
void exit_g(params_t *prms);
void pause(params_t *prms);
void resume(params_t *prms);
void restart(params_t *prms);

void clean_map(params_t *prms);
void delete_rows(params_t *prms, int *rows, int count);
void upd_time(params_t *prms);

void add_points(params_t *prms, int r_amount);
void level_up(params_t *prms);

//  1    START = 0,
//  2    SPAWN,
//  3    MOVING,
//  4    SHIFTING,
//  5    ATTACHING,
//  6    GAMEOVER,
//  7    EXIT_STATE,
//  8    PAUSE,
//  9    FILE_ERROR_STATE

/**
 * \brief Action table
 * \details Check 'sigact' function to know
 * \version 1.0
 * \author vylarrca
*/
action fsm_table[8][7] = {
    {NULL, NULL, NULL, NULL, start_g, exit_g, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {upd_time, move_down, move_right, move_left, rotate, pause, upd_time},

    {shift, shift, shift, shift, shift, shift, shift},
    {attach, attach, attach, attach, attach, attach, attach},
    {gam_ov, gam_ov, gam_ov, gam_ov, restart, exit_g, gam_ov},

    {exit_g, exit_g, exit_g, exit_g, exit_g, exit_g, exit_g},
    {pause, pause, pause, pause, resume, exit_g, pause}};

/**
 * \brief Call one function from fsm_table
 * \details The call depends on the current state and the user signal, 
 * for exaple if user push 'd' while state == MOVING sigact call move_right function
 * \version 1.0
 * \author vylarrca
*/
void sigact(signals sig, game_state *state, game_stats_t *stats, board_t *map,
            tile *current, tile *next, tile *tiles) {
  action act = NULL;
  params_t params = compress_params(state, stats, map, current, next, tiles);

  if (*state != FILE_ERROR_STATE) act = fsm_table[*state][sig];

  if (act != NULL) act(&params);
}

/**
 * \brief Return const of usr input
 * \details If there no correct inpuct return NOSIG, other way one of the 'MOVE_' consts
 * \version 1.0
 * \author vylarrca
*/
signals get_signal(int user_input) {
  signals sig = NOSIG;

  if (user_input == KEY_UP)
    sig = MOVE_SLOW;
  else if (user_input == KEY_D)
    sig = MOVE_FAST;
  else if (user_input == KEY_R)
    sig = MOVE_RIGHT;
  else if (user_input == KEY_L)
    sig = MOVE_LEFT;
  else if (user_input == KEY_S)
    sig = ROTATE_BTN;
  else if (user_input == KEY_ESCAPE)
    sig = ESCAPE_BTN;

  return sig;
}

/**
 * \brief Some preparing for next state
 * \details Set next_tile and set SPAWN state
 * \version 1.0
 * \author vylarrca
*/
void start_g(params_t *prms) {
  *prms->next_tile = get_rand_tile(prms->all_tiles);
  *prms->state = SPAWN;
}

/**
 * \brief Spawn a tile
 * \details Set tile from next tile and generate next tile again
 * \version 1.0
 * \author vylarrca
*/
void spawn(params_t *prms) {
  if (prms->next_tile != NULL) {
    *prms->tile = *prms->next_tile;
    prms->tile->x = MAP_H / 2 - TILE_SIZE / 2;
    prms->tile->y = 0;
    *prms->next_tile = get_rand_tile(prms->all_tiles);
    *prms->state = MOVING;
  }
  if (!check_falling(prms) || check_collision(prms)) *prms->state = ATTACHING;
}

/**
 * \brief Check collision of current tile and other falled tiles
 * \version 1.0
 * \author vylarrca
*/
bool check_collision(params_t *prms) {
  bool is_collide = false;
  int x = prms->tile->x;
  int y = prms->tile->y;
  for (int i = 0; i < TILE_SIZE && !is_collide; ++i) {
    for (int j = 0; j < TILE_SIZE && !is_collide; ++j) {
      int t_cell = prms->tile->container[i][j];
      int m_cell = prms->map->map_ar[y + i][x + j];
      if (t_cell == m_cell && t_cell == TILE_CHAR) {
        is_collide = true;
      }
    }
  }

  return is_collide;
}

/**
 * \brief Is tile falling?
 * \details Returs bool value true when nothing below the current tile, false at the opposite
 * \version 1.0
 * \author vylarrca
*/
bool check_falling(params_t *prms) {
  bool is_falling = true, tile_found;
  int bottom_x = 0, bottom_y = 0;

  if (prms->tile->y + prms->tile->size_y >= MAP_V) {
    is_falling = false;
  }
  for (int i = 0; i < TILE_SIZE && is_falling; ++i) {
    bottom_x = 0, bottom_y = 0;
    tile_found = false;
    for (int j = TILE_SIZE - 1; !tile_found && j >= 0; --j) {
      if (prms->tile->container[j][i] == TILE_CHAR) {
        bottom_x = i, bottom_y = j;
        tile_found = true;
      }
    }
    int tmp_x = prms->tile->x + bottom_x;
    int tmp_y = prms->tile->y + bottom_y + 1;
    int tmp_cell = prms->map->map_ar[tmp_y][tmp_x];
    if ((tmp_cell == TILE_CHAR || tmp_y >= MAP_V) && tile_found) {
      is_falling = false;
    }
  }
  return is_falling;
}

/**
 * \brief Down the tile if it possible
 * \version 1.0
 * \author vylarrca
*/
void shift(params_t *prms) {
  if (!check_falling(prms)) {
    *prms->state = ATTACHING;
  } else {
    prms->tile->y += 1;
    *prms->state = MOVING;
  }
}

/**
 * \brief Attach the current tile if it stops falling
 * \details Check if attach with no shift than GAMEOVER. Also run clean_map function
 * \version 1.0
 * \author vylarrca
*/
void attach(params_t *prms) {
  int x = prms->tile->x;
  int y = prms->tile->y;
  if (check_collision(prms) || (y == 0 && !check_falling(prms))) {
    *prms->state = GAMEOVER;
  } else if (!check_falling(prms)) {
    for (int i = 0; i < TILE_SIZE; ++i) {
      for (int j = 0; j < TILE_SIZE; ++j) {
        if (prms->tile->container[i][j] == TILE_CHAR) {
          prms->map->map_ar[y + i][x + j] = prms->tile->container[i][j];
        }
      }
    }
    *prms->state = SPAWN;
    clean_map(prms);
  }
}

/**
 * \brief Make shift time closer
 * \version 1.0
 * \author vylarrca
*/
void move_down(params_t *prms) {
  prms->stats->shift_time -= SHIFT_TIME / (prms->stats->level + 2);
}

/**
 * \brief prms->tile->x += 1;
 * \details Move the tile right if it possible
 * \version 1.0
 * \author vylarrca
*/
void move_right(params_t *prms) {
  bool is_movable = true, tile_found = false;
  int right_x = 0, right_y = 0;

  if (prms->tile->x + prms->tile->size_x >= MAP_H) {
    is_movable = false;
  }

  for (int i = 0; i < TILE_SIZE && is_movable; ++i) {
    right_x = 0, right_y = 0;
    tile_found = false;
    for (int j = TILE_SIZE - 1; !tile_found && j >= 0; --j) {
      if (prms->tile->container[i][j] == TILE_CHAR) {
        right_x = j, right_y = i;
        tile_found = true;
      }
    }
    int tmp_x = prms->tile->x + right_x + 1;
    int tmp_y = prms->tile->y + right_y;
    if (prms->map->map_ar[tmp_y][tmp_x] == TILE_CHAR && tile_found) {
      is_movable = false;
    }
  }

  if (is_movable) {
    prms->tile->x += 1;
  }
}

/**
 * \brief prms->tile->x -= 1;
 * \details Move the tile left if it possible
 * \version 1.0
 * \author vylarrca
*/
void move_left(params_t *prms) {
  bool is_movable = true, tile_found = false;
  int left_x = 0, left_y = 0;

  if (prms->tile->x <= 0) {
    is_movable = false;
  }

  for (int i = 0; i < TILE_SIZE && is_movable; ++i) {
    left_x = 0, left_y = 0;
    tile_found = false;
    for (int j = 0; j < TILE_SIZE && !tile_found; ++j) {
      if (prms->tile->container[i][j] == TILE_CHAR) {
        left_x = j, left_y = i;
        tile_found = true;
      }
    }
    int tmp_x = prms->tile->x + left_x - 1;
    int tmp_y = prms->tile->y + left_y;
    if (prms->map->map_ar[tmp_y][tmp_x] == TILE_CHAR && tile_found) {
      is_movable = false;
    }
  }

  if (is_movable) {
    prms->tile->x -= 1;
  }
}

/**
 * \brief Rotate a tile
 * \details Call tile_rotate
 * \version 1.0
 * \author vylarrca
*/
void rotate(params_t *prms) { tile_rotate(prms->tile); }

/**
 * \brief Make the game over
 * \details Print banner and set the correct state
 * \version 1.0
 * \author vylarrca
*/
void gam_ov(params_t *prms) {
  print_game_over();
  *prms->state = GAMEOVER;
}

/**
 * \brief Make the game over (really)
 * \details Save stats and bye ;)
 * \version 1.0
 * \author vylarrca
*/
void exit_g(params_t *prms) {
  *prms->state = EXIT_STATE;
  save_stats(*prms->stats);
}

/**
 * \brief Make the game pause
 * \details Print banner and set the correct state
 * \version 1.0
 * \author vylarrca
*/
void pause(params_t *prms) {
  print_pause();
  *prms->state = PAUSE;
}

/**
 * \brief Make the game run
 * \details Close banner and set the correct state, get new shift_time.
 * \version 1.0
 * \author vylarrca
*/
void resume(params_t *prms) {
  *prms->state = MOVING;
  prms->stats->cur_time = get_time();
  prms->stats->shift_time =
      prms->stats->cur_time + SHIFT_TIME / (prms->stats->level + 1);
}

/**
 * \brief Make the game run
 * \details Reset stats except bestscore. state = START
 * \version 1.0
 * \author vylarrca
*/
void restart(params_t *prms) {
  reset_map(prms->map);
  prms->stats->level = 0;
  prms->stats->score = 0;
  *prms->state = START;
}

/**
 * \brief Find all fulfilled rows and delete it
 * \details Calls the add_points and delete_rows functions
 * \version 1.0
 * \author vylarrca
*/
void clean_map(params_t *prms) {
  bool row_is_full = true;
  int full_rows[MAP_V];
  int k = 0;
  for (int i = 0; i < MAP_V; ++i) {
    row_is_full = true;
    for (int j = 0; j < MAP_H; ++j) {
      if (prms->map->map_ar[i][j] == SPACE_CHAR) {
        row_is_full = false;
      }
    }
    if (row_is_full) {
      full_rows[k] = i;
      ++k;
    }
  }
  add_points(prms, k);
  delete_rows(prms, full_rows, k);
}

/**
 * \brief Delete rows in list
 * \version 1.0
 * \author vylarrca
*/
void delete_rows(params_t *prms, int *rows, int count) {
  // blink rows
  int new_i = MAP_V - 1, row_ind = count - 1;
  // prms->map->map_ar[row_ind][0] = TILE_CHAR;

  for (int i = MAP_V - 1; i >= 0 && count > 0; --i) {
    for (int j = 0; j < MAP_H && i != rows[row_ind]; ++j) {
      prms->map->map_ar[new_i][j] = prms->map->map_ar[i][j];
    }
    if (i != rows[row_ind]) {
      --new_i;
    }
    if (i == rows[row_ind] && row_ind >= 0) {
      --row_ind;
    }
  }
  for (int i = new_i; i >= 0 && count > 0; --i) {
    for (int j = 0; j < MAP_H && i != rows[row_ind]; ++j) {
      prms->map->map_ar[new_i][j] = prms->map->map_ar[i][j];
    }
  }
}

/**
 * \brief Update current time
 * \details Calls get_time() function
 * \version 1.0
 * \author vylarrca
*/
void upd_time(params_t *prms) { prms->stats->cur_time = get_time(); }

/**
 * \brief Add score
 * \details check level up
 * \version 1.0
 * \author vylarrca
*/
void add_points(params_t *prms, int r_amount) {
  switch (r_amount) {
    case 1:
      prms->stats->score += 100;
      break;
    case 2:
      prms->stats->score += 300;
      break;
    case 3:
      prms->stats->score += 700;
      break;
    case 4:
      prms->stats->score += 1500;
      break;
  }
  if (prms->stats->score > prms->stats->best_score)
    prms->stats->best_score = prms->stats->score;
  level_up(prms);
}

/**
 * \brief Let you level up
 * \details check score and give you a level
 * \version 1.0
 * \author vylarrca
*/
void level_up(params_t *prms) {
  prms->stats->level = prms->stats->score / 600;
  if (prms->stats->level > 10) prms->stats->level = 10;
}