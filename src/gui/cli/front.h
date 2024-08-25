#ifndef FRONT_H
#define FRONT_H

#include "../../brick_game/tetris/tetris.h"

void print_tet(TGame *tetg, WINDOW *board);
void print_figure(TFigure *tetg, int i, int j, WINDOW *board);
void print_rec(TGame *tetg, WINDOW *record);
void print_fig(TGame *tetg, WINDOW *next_fig);
void print_pause(WINDOW *board);
void print_game_over(TGame *tetg, WINDOW *board);
void print_start(TGame *tetg, WINDOW *board);
void print_front(TGame *tetg, Windows *win);
Windows *init_win();
void init_ncurses();

#endif