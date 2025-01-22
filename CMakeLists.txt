cmake_minimum_required(VERSION 3.22)
project(C7_BrickGame_v1_0_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(src/brick_game/tetris)
include_directories(src/gui/cli)
target_link_libraries(<target> PRIVATE "-lncurses")

add_executable(C7_BrickGame_v1_0_1
        src/brick_game/tetris/backend.c
        src/brick_game/tetris/backend.h
        src/brick_game/tetris/fsm.c
        src/brick_game/tetris/fsm.h
        src/brick_game/tetris/tetris.c
        src/brick_game/tetris/tetris.h
        src/gui/cli/frontend.c
        src/gui/cli/frontend.h)
