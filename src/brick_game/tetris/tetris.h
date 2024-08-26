#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define COUNT 7
#define WIDTH 10
#define HIGHT 25

// Структура которая показывает наличеи или отсутствие блока
typedef struct TBlock {
  int n;
} TBlock;

// Структура одной фигуры
typedef struct TFigure {
  int x;
  int y;
  TBlock *block;
} TFigure;

// Структура возможных фигур
typedef struct TFigures {
  TBlock *blocks;
} TFigures;

// Структура игрового поля
typedef struct TField {
  TBlock *blocks;
} TField;

// Список состояний игры
enum { GAMEOVER = 0, START, PLAYING, PAUSE, LEFT, RIGHT, DOWN, TURN };

// Структура параметров игры
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

// Структа окон игры
typedef struct Windows {
  WINDOW *board;
  WINDOW *box;
  WINDOW *record;
  WINDOW *next_fig;
} Windows;

// Инициализация структур
TGame *start_Game();
TField *build_field();
TFigures *build_figures(TBlock *temp_fig);
TFigure *build_fig();

// Удаление выделенной памяти
void freeTGame(TGame *tetg);
void freeTField(TField *tetf);
void freeTFigures(TFigures *tetfig);
void freeTFigure(TFigure *tetf);

// Действия игрока
void action_play(TGame *tetg, int chr);
TFigure *turn_fig(TGame *tetg);
void move_up(TGame *tetg);
void move_down(TGame *tetg);
void move_left(TGame *tetg);
void move_right(TGame *tetg);
void key_up(TGame *tetg);

void tact_game(TGame *tetg);
int clash(TGame *tetg);
int compound_fig(TGame *tetg);
int deleteline(TGame *tetg);
int full_line(TField *tetf, int i);
void line_down(TField *tetf, int i);
void drop_fig(TGame *tetg);
void color_block(TField *tetf, TFigure *fig, int i, int j);
int level_up(TGame *tetg);

// Чтение и запись рекорда
int read_record();
void write_record(TGame *tetg);

#endif