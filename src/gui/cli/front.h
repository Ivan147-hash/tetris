#ifndef FRONT_H
#define FRONT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "../../brick_game/tetris/tetris.h"

void print_tet(TGame *tetg, WINDOW *board);

void print_rec(TGame *tetg, WINDOW *record);

void print_fig(TGame *tetg, WINDOW *next_fig);

void print_pause(TGame *tetg, WINDOW *board);

void print_game_over(TGame *tetg, WINDOW *board);

void print_start(TGame *tetg, WINDOW *board);

void print_front(TGame *tetg, Windows *win);

#endif