#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define WIDTH 10
#define HIGHT 25

typedef struct TBlock {
  int n;
} TBlock;

typedef struct TFigure {
  int x;
  int y;
  TBlock *block;
} TFigure;

typedef struct TFigures {
  int count;
  TBlock *blocks;
} TFigures;

typedef struct TField {
  // int width;
  // int height;
  TBlock *blocks;
} TField;

enum { GAMEOVER = 0, START, PLAYING, PAUSE, LEFT, RIGHT, DOWN, TURN };

typedef struct TGame {
  TField *field;
  TFigure *figure;
  TFigures *figures;
  int score;
  int hard_score;
  int fig;
  int next_fig;
  int status;
  int level;
} TGame;

typedef struct Windows {
  WINDOW *board;
  WINDOW *record;
  WINDOW *next_fig;
} Windows;

TGame *start_Game(int count);

void freeTGame(TGame *tetg);

TField *build_field();

void freeTField(TField *tetf);

TFigures *build_figures(TBlock *temp_fig, int count);

void freeTFigures(TFigures *tetfig);

void tact_game(TGame *tetg);

void move_up(TGame *tetg);

void move_down(TGame *tetg);

void move_left(TGame *tetg);

void move_right(TGame *tetg);

int clash(TGame *tetg);

int compound_fig(TGame *tetg);

int deleteline(TGame *tetg);

int full_line(TField *tetf, int i);

void line_down(TField *tetf, int i);

void drop_fig(TGame *tetg);

TFigure *build_fig();

void freeTFigure(TFigure *tetf);

TFigure *turn_fig(TGame *tetg);

void print_tet(TGame *tetg, WINDOW *window);

void color_block(TField *tetf, TFigure *fig, int i, int j);

int read_record();

void write_record(TGame *tetg);

int level_up(TGame *tetg);

void key_up(TGame *tetg);

void action_play(TGame *tetg, int chr);

#endif