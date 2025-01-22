/**
 * \file
 * \brief .C file with realizations of backend.h
 * \details This file contain all realizations
*/
#include "backend.h"

/**
 * \brief Rotate a tile to the right
 * \details Just rotate the container and returns it to the left up angle
 * \bug Don't check what happend after rotation, careful. 
 * \version 1.0
 * \author vylarrca
*/
void tile_rotate(tile *self) {
  tile temp;
  int x = TILE_SIZE, y = TILE_SIZE;
  for (int i = 0; i < TILE_SIZE; i++) {
    for (int j = 0; j < TILE_SIZE; j++) {
      temp.container[i][j] = self->container[TILE_SIZE - j - 1][i];

      if (temp.container[i][j] != SPACE_CHAR && j < x) x = j;
      if (temp.container[i][j] != SPACE_CHAR && i < y) y = i;
    }
  }
  for (int i = 0; i < TILE_SIZE; i++) {
    for (int j = 0; j < TILE_SIZE; j++) {
      self->container[i][j] =
          temp.container[(i + y) % TILE_SIZE][(j + x) % TILE_SIZE];
    }
  }
  x = self->size_x;
  self->size_x = self->size_y;
  self->size_y = x;
}

/**
 * \brief Copy tile's container
 * \details Matrix copier loop
 * \version 1.0
 * \author vylarrca
*/
void tile_copymap(tile *dst, tile src) {
  for (int i = 0; i < TILE_SIZE; i++) {
    for (int j = 0; j < TILE_SIZE; j++) {
      dst->container[i][j] = src.container[i][j];
    }
  }
}

/**
 * \brief Copy tile
 * \details From src to dst, returns nothing
 * \version 1.0
 * \author vylarrca
*/
void tile_copy(tile *dst, tile src) {
  tile_copymap(dst, src);
  dst->size_x = src.size_x;
  dst->size_y = src.size_y;
  dst->x = src.x;
  dst->y = src.y;
}

/**
 * \brief Reset tile by default
 * \details Set SPACE_CHAR to every part in tile.container
 * \version 1.0
 * \author vylarrca
*/
void tile_zero(tile *self) {
  for (int i = 0; i < TILE_SIZE; ++i) {
    for (int j = 0; j < TILE_SIZE; ++j) {
      self->container[i][j] = SPACE_CHAR;
    }
  }
}

/**
 * \brief Set all base tiles 
 * \details That's all what I can say... ;) (No dynamic memory)
 * \version 1.0
 * \author vylarrca
*/
void tile_set(tile *tile_list) {
  tile_zero(&tile_list[0]);
  tile_zero(&tile_list[1]);
  tile_zero(&tile_list[2]);
  tile_zero(&tile_list[3]);
  tile_zero(&tile_list[4]);
  tile_zero(&tile_list[5]);
  tile_zero(&tile_list[6]);
  printf("1\n");

  tile_list[0].container[0][0] = TILE_CHAR;
  tile_list[0].container[0][1] = TILE_CHAR;
  tile_list[0].container[0][2] = TILE_CHAR;
  tile_list[0].container[0][3] = TILE_CHAR;
  tile_list[0].size_x = 4;
  tile_list[0].size_y = 1;

  tile_list[1].container[0][0] = TILE_CHAR;
  tile_list[1].container[1][0] = TILE_CHAR;
  tile_list[1].container[1][1] = TILE_CHAR;
  tile_list[1].container[1][2] = TILE_CHAR;
  tile_list[1].size_x = 3;
  tile_list[1].size_y = 2;

  tile_list[2].container[1][0] = TILE_CHAR;
  tile_list[2].container[1][1] = TILE_CHAR;
  tile_list[2].container[1][2] = TILE_CHAR;
  tile_list[2].container[0][2] = TILE_CHAR;
  tile_list[2].size_x = 3;
  tile_list[2].size_y = 2;

  tile_list[3].container[0][0] = TILE_CHAR;
  tile_list[3].container[0][1] = TILE_CHAR;
  tile_list[3].container[1][0] = TILE_CHAR;
  tile_list[3].container[1][1] = TILE_CHAR;
  tile_list[3].size_x = 2;
  tile_list[3].size_y = 2;

  tile_list[4].container[0][1] = TILE_CHAR;
  tile_list[4].container[0][2] = TILE_CHAR;
  tile_list[4].container[1][0] = TILE_CHAR;
  tile_list[4].container[1][1] = TILE_CHAR;
  tile_list[4].size_x = 3;
  tile_list[4].size_y = 2;

  tile_list[5].container[0][1] = TILE_CHAR;
  tile_list[5].container[1][0] = TILE_CHAR;
  tile_list[5].container[1][1] = TILE_CHAR;
  tile_list[5].container[1][2] = TILE_CHAR;
  tile_list[5].size_x = 3;
  tile_list[5].size_y = 2;

  tile_list[6].container[0][0] = TILE_CHAR;
  tile_list[6].container[0][1] = TILE_CHAR;
  tile_list[6].container[1][1] = TILE_CHAR;
  tile_list[6].container[1][2] = TILE_CHAR;
  tile_list[6].size_x = 3;
  tile_list[6].size_y = 2;
  printf("1\n");
}

/**
 * \brief init dynamic memory
 * \details Call calloc to ptr
 * \param tile_list Have to be pointer to *var (example: T *p; init_mem_tile_list(&p);)
 * \version 1.0
 * \author vylarrca
*/
void init_mem_tile_list(tile **tile_list) { 
  *tile_list = (tile *)calloc(TILE_AMOUNT, sizeof(tile));
}

/**
 * \brief Choose one from tile_list and return it
 * \details Works by function get_time(), get remainder of the division by TILE_AMOUNT
 * \version 1.0
 * \author vylarrca
*/
tile get_rand_tile(tile *tile_list) {
  return tile_list[get_time() % TILE_AMOUNT];
}

/**
 * \brief Free tile_list
 * \details Capsule for free(tile_list) to make code more readable
 * \version 1.0
 * \author vylarrca
*/
void tile_destroy(tile *tile_list) { free(tile_list); }

/**
 * \brief Loading function
 * \details Zero all game important variables
 * \version 1.0
 * \author vylarrca
*/
void stats_init(game_stats_t *stats) {
  // load best score
  load_stats(stats);
  stats->level = 0;
  stats->score = 0;
  stats->cur_time = get_time();
  stats->shift_time = stats->cur_time + SHIFT_TIME / (stats->level + 1);
}

/**
 * \brief Load statistics from a txt file
 * \details saves.txt
 * \version 1.0
 * \author vylarrca
*/
void load_stats(game_stats_t *stats) {
  FILE *file = fopen("./saves.txt", "r");

  if (file) {
    fscanf(file, "%d", &stats->best_score);
    fclose(file);
  } else {
    stats->best_score = 0;
  }
}

/**
 * \brief Saves statistics to a txt file
 * \details saves.txt
 * \version 1.0
 * \author vylarrca
*/
void save_stats(game_stats_t stats) {
  FILE *file = fopen("./saves.txt", "w+");

  if (file) {
    fprintf(file, "%d\n", stats.best_score);
    fclose(file);
  }
}

/**
 * \brief Returns time in milliseconds
 * \details The get_time() method of a Date object returns the numeric value of the specified date as the number of milliseconds
 * passed since 00:00:00 January 1, 1970 UTC
 * \version 1.0
 * \author vylarrca
*/
long get_time() {
  struct timeval A;
  gettimeofday(&A, NULL);
  return A.tv_sec * 1000 + A.tv_usec / 1000;
}

/**
 * \brief Combines pointers to different variables into one structure
 * \details Метод get_time() объекта Date возвращает числовое значение указанной даты в виде количества миллисекунд, 
 * прошедших с 00:00:00 1 января 1970 года по UTC
 * \version 1.0
 * \author vylarrca
*/
params_t compress_params(game_state *state, game_stats_t *stats, board_t *map,
                         tile *current, tile *next, tile *tiles) {
  params_t params;
  params.state = state;
  params.stats = stats;
  params.map = map;
  params.next_tile = next;
  params.tile = current;
  params.all_tiles = tiles;
  return params;
}
