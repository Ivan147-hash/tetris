#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

typedef struct TBlock {
    int n;
} TBlock;

typedef struct TFigure {
    int x;
    int y;
    int size;
    TBlock *block;
} TFigure;

typedef struct TFigures {
    int count;
    int size;
    TBlock *blocks;
} TFigures;

typedef struct TField{
    int width;
    int height;
    TBlock *blocks;
} TField;

enum {
    GAMEOVER = 0,
    START,
    PLAYING,
    PAUSE, 
    LEFT,
    RIGHT,
    DOWN,
    UP
};

enum {
    BUT_NON = 0,
    BUT_F,
    BUT_DOWN,
    BUT_LEFT,
    BUT_RIGHT
};

typedef struct TPlayer {
    int action;
} TPlayer;

typedef struct TGame{
    TField *field;
    TFigure *figure;
    TFigures *figures;
    int score;
    int hard_score;
    int fig;
    int next_fig;
    int status;
    int level;
    TPlayer *player;
} TGame;

typedef struct Windows{
    WINDOW *board ;
    WINDOW *record;
    WINDOW *next_fig;
} Windows;

// TBlock all_blocks[] = {
//     0, 0, 1, 0, 0,
//     0, 0, 1, 0, 0,
//     0, 0, 1, 0, 0,
//     0, 0, 1, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 0, 1, 1, 0,
//     0, 0, 1, 1, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 1, 0, 0, 0,
//     0, 1, 1, 1, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 0, 0, 1, 0,
//     0, 1, 1, 1, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 0, 1, 1, 0,
//     0, 1, 1, 0, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 0, 1, 0, 0,
//     0, 1, 1, 1, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,

//     0, 0, 0, 0, 0,
//     0, 1, 1, 0, 0,
//     0, 0, 1, 1, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,
// };

TGame* start_Game(int width, int height, int size, int count, TBlock *temp_fig);

void freeTGame(TGame *tetg, Windows *win);

TField* build_field(int width, int height);

void freeTField(TField *tetf);

TFigures* build_figures(TBlock *temp_fig, int count, int size);

void freeTFigures(TFigures *tetfig);

void tact_game(TGame *tetg);

void move_up(TGame *tetg);

void move_down(TGame *tetg);

void move_left(TGame *tetg);

void move_right(TGame *tetg);

int clash(TGame *tetg);

void compound_fig(TGame *tetg);

int deleteline(TGame *tetg);

int full_line(TField *tetf, int i);

void line_down(TField *tetf, int i);

void drop_fig(TGame *tetg);

TFigure* build_fig(TGame *tetg);

void freeTFigure(TFigure *tetf);

TFigure* turn_fig(TGame *tetg);

void print_tet(TGame *tetg, WINDOW *window);

void color_block(TField *tetf, TFigure *fig, int i, int j);

int read_record();

void write_record(TGame *tetg);

int level_up(TGame *tetg);

void action_play(TGame *tetg, WINDOW *board);

Windows* init_win();

void freeWindows(Windows *win);

void init_ncurses();

#endif