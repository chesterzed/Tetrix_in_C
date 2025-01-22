#include <check.h>
#include "tests.h"

START_TEST(s21_bg_visual) {
    game_state st = START;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);
    params_t p = compress_params(&st, &stats, &map, &current, &next, tiles);

    // init_visual();

    upd_borad(p);
    ck_assert_int_eq(st, START);
    ck_assert_int_eq(board[1][4], BOARD_CHAR);
    
    tile_destroy(tiles);
    endwin();
}

START_TEST(s21_bg_pause) {
    game_state st = PAUSE;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;
    for (int i = 3; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);

    
    ck_assert_int_eq(st, PAUSE);
    tile_destroy(tiles);
}

START_TEST(s21_bg_restart) {
    game_state st = GAMEOVER;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_S;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;
    for (int i = 3; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);

    
    ck_assert_int_eq(st, START);
    ck_assert_int_eq(stats.level, 0);
    ck_assert_int_eq(stats.score, 0);
    tile_destroy(tiles);
}

START_TEST(s21_bg_down) {
    game_state st = MOVING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_D;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    stats.shift_time = get_time();
    long int t = stats.shift_time;
    int b = 0;

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;
    for (int i = 3; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);

    if (t > stats.shift_time)
        b = 1;
    
    ck_assert_int_eq(b, 1);
    tile_destroy(tiles);
}

START_TEST(s21_bg_shift1) {
    game_state st = SHIFTING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_D;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 1;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    // ck_assert_int_eq(st, MOVING);
    ck_assert_int_eq(current.y, 3);
    tile_destroy(tiles);
}

START_TEST(s21_bg_shift2) {
    game_state st = SHIFTING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_ESCAPE;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;
    for (int i = 3; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(st, ATTACHING);
    tile_destroy(tiles);
}

START_TEST(s21_bg_exit) {
    game_state st = EXIT_STATE;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_ESCAPE;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(st, EXIT_STATE);
    tile_destroy(tiles);
}

START_TEST(s21_bg_start) {
    game_state st = START;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_S;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&current);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;

    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(st, SPAWN);
    tile_destroy(tiles);
}

START_TEST(s21_bg_resume) {
    game_state st = PAUSE;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_S;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&next);

    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;

    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(st, MOVING);
    tile_destroy(tiles);
}

START_TEST(s21_bg_left) {
    game_state st = MOVING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_L;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&next);
    next.container[0][1] = TILE_CHAR;
    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;

    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(current.x, 9);
    tile_destroy(tiles);
}

START_TEST(s21_bg_right) {
    game_state st = MOVING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    tile_zero(&next);
    current.y = 2;
    current.x = 10;
    current.container[0][0] = TILE_CHAR;

    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(current.x, 11);
    tile_destroy(tiles);
}

START_TEST(s21_bg_attach1) {
    game_state st = ATTACHING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    current.y = 2;
    current.container[0][0] = TILE_CHAR;

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    for (int i = 3; i < 4; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], SPACE_CHAR);
    ck_assert_int_eq(st, SPAWN);
    ck_assert_int_eq(current.y, 2);
    tile_destroy(tiles);
}

START_TEST(s21_bg_attach2) {
    game_state st = ATTACHING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    current.y = 2;
    current.container[0][0] = TILE_CHAR;

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    for (int i = 3; i < 5; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], SPACE_CHAR);
    ck_assert_int_eq(st, SPAWN);
    ck_assert_int_eq(current.y, 2);
    tile_destroy(tiles);
}

START_TEST(s21_bg_attach3) {
    game_state st = ATTACHING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    current.y = 2;
    current.container[0][0] = TILE_CHAR;

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    for (int i = 3; i < 6; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], SPACE_CHAR);
    ck_assert_int_eq(st, SPAWN);
    ck_assert_int_eq(current.y, 2);
    tile_destroy(tiles);
}

START_TEST(s21_bg_attach4) {
    game_state st = ATTACHING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_R;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    current.y = 2;
    current.container[0][0] = TILE_CHAR;

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    for (int i = 3; i < 7; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], SPACE_CHAR);
    ck_assert_int_eq(st, SPAWN);
    ck_assert_int_eq(current.y, 2);
    tile_destroy(tiles);
}

START_TEST(s21_bg_attach_over) {
    game_state st = ATTACHING;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_UP;
    init_mem_tile_list(&tiles);
    tile_set(tiles);

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = SPACE_CHAR;
        }
    }
    for (int i = MAP_V - 6; i < MAP_V - 3; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], SPACE_CHAR);
    ck_assert_int_eq(st, GAMEOVER);
    ck_assert_int_eq(current.y, 0);
    tile_destroy(tiles);
}


START_TEST(s21_bg_collide) {
    game_state st = SPAWN;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    tile next;
    tile current;
    tile *tiles;
    int sig = KEY_L;
    init_mem_tile_list(&tiles);
    tile_set(tiles);

    next = get_rand_tile(tiles);
    for (int i = 0; i < MAP_V; ++i) {
        for (int j = 0; j < MAP_H; ++j) {
            map.map_ar[i][j] = TILE_CHAR;
        }
    }
    sigact(get_signal(sig), &st, &stats, &map, &current, &next, tiles);
    
    
    ck_assert_int_eq(map.map_ar[5][6], TILE_CHAR);
    ck_assert_int_eq(st, ATTACHING);
    ck_assert_int_eq(current.y, 0);
    tile_destroy(tiles);
}

START_TEST(s21_bg_rotate) {
    tile t1;
    tile_zero(&t1);
    t1.container[0][1] = TILE_CHAR;
    tile_rotate(&t1);
    ck_assert_int_eq(t1.container[0][0], TILE_CHAR);
}

START_TEST(s21_bg_copy) {
    tile t1, t2;
    tile_zero(&t1);
    t1.container[0][0] = TILE_CHAR;
    tile_copy(&t2, t1);
    ck_assert_int_eq(t2.container[0][0], TILE_CHAR);
}

START_TEST(s21_bg_load_fail) {
    game_stats_t st;
    load_stats(&st);
    ck_assert_int_eq(st.best_score, 0);
}

START_TEST(s21_bg_init_tile_list) {
    tile *tiles;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    ck_assert_int_eq(tiles[0].container[0][0], TILE_CHAR);
    ck_assert_int_eq(tiles[1].container[1][0], TILE_CHAR);
    ck_assert_int_eq(tiles[2].container[1][1], TILE_CHAR);
    ck_assert_int_eq(tiles[3].container[1][1], TILE_CHAR);
    ck_assert_int_eq(tiles[4].container[0][2], TILE_CHAR);
    ck_assert_int_eq(tiles[5].container[1][0], TILE_CHAR);
    ck_assert_int_eq(tiles[6].container[1][2], TILE_CHAR);
    tile_destroy(tiles);
}

START_TEST(s21_bg_save_load) {
    game_stats_t stats, stats2;
    stats.best_score = 500;
    save_stats(stats);
    load_stats(&stats2);
    ck_assert_int_eq(stats.best_score, stats2.best_score);
}


START_TEST(s21_bg_get_time) {
    int i = 0;
    if (get_time() > 0)
        i = 1;

  ck_assert_int_eq(i, 1);
}

START_TEST(s21_bg_compress_params) {
    game_state st = START;
    game_stats_t stats;
    stats_init(&stats);
    board_t map;
    params_t params;
    tile next;
    tile current;
    tile *tiles;
    init_mem_tile_list(&tiles);
    tile_set(tiles);
    params = compress_params(&st, &stats, &map, &current, &next, tiles);
    
    ck_assert_int_eq(params.stats->best_score, stats.best_score);
    ck_assert_int_eq(params.stats->score, stats.score);
    ck_assert_int_eq(params.stats->level, stats.level);
    ck_assert_int_eq(params.all_tiles[4].container[0][2], TILE_CHAR);
    tile_destroy(tiles);
}

Suite *s21_test_group_create() {
  Suite *s21_test_group = suite_create("s21_test_group");
  TCase *s21_bg_tests = tcase_create("S21_BG_TESTS");

//   tcase_add_test(s21_bg_tests, s21_bg_test_name);
//   tcase_add_test(s21_bg_tests, s21_bg_test_name);
//   tcase_add_test(s21_bg_tests, s21_bg_test_name);
  tcase_add_test(s21_bg_tests, s21_bg_visual);
  tcase_add_test(s21_bg_tests, s21_bg_pause);
  tcase_add_test(s21_bg_tests, s21_bg_restart);
  tcase_add_test(s21_bg_tests, s21_bg_down);
  tcase_add_test(s21_bg_tests, s21_bg_shift1);
  tcase_add_test(s21_bg_tests, s21_bg_shift2);
  tcase_add_test(s21_bg_tests, s21_bg_exit);
  tcase_add_test(s21_bg_tests, s21_bg_start);
  tcase_add_test(s21_bg_tests, s21_bg_resume);
  tcase_add_test(s21_bg_tests, s21_bg_left);
  tcase_add_test(s21_bg_tests, s21_bg_right);
  tcase_add_test(s21_bg_tests, s21_bg_attach1);
  tcase_add_test(s21_bg_tests, s21_bg_attach2);
  tcase_add_test(s21_bg_tests, s21_bg_attach3);
  tcase_add_test(s21_bg_tests, s21_bg_attach4);
  tcase_add_test(s21_bg_tests, s21_bg_attach_over);
  tcase_add_test(s21_bg_tests, s21_bg_collide);
  tcase_add_test(s21_bg_tests, s21_bg_rotate);
  tcase_add_test(s21_bg_tests, s21_bg_copy);
  tcase_add_test(s21_bg_tests, s21_bg_load_fail);
  tcase_add_test(s21_bg_tests, s21_bg_init_tile_list);
  tcase_add_test(s21_bg_tests, s21_bg_save_load);
  tcase_add_test(s21_bg_tests, s21_bg_get_time);
  tcase_add_test(s21_bg_tests, s21_bg_compress_params);

  suite_add_tcase(s21_test_group, s21_bg_tests);

  return s21_test_group;
}

int main() {
  Suite *s21_brick_game = s21_test_group_create();
  SRunner *s21_brick_game_runner = srunner_create(s21_brick_game);
  srunner_run_all(s21_brick_game_runner, CK_NORMAL);
  srunner_ntests_failed(s21_brick_game_runner);
  srunner_free(s21_brick_game_runner);

  return 0;
}