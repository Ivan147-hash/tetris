#include "tetris.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef TETRIS_C
#define TETRIS_C

TFigures* build_figures(TBlock *temp_fig, int count, int size) {
    TFigures *tetfig = (TFigures*)malloc(sizeof(TFigures));
    tetfig->count = count;
    tetfig->size = size;
    tetfig->blocks = temp_fig;
    return tetfig;
}

void freeTFigures(TFigures *tetfig) {
    if (tetfig) {
        free(tetfig);
    }
}

TField* build_field(int width, int height) {
    TField *tetf = (TField*)malloc(sizeof(TField));
    tetf->width = width;
    tetf->height = height;
    tetf->blocks = (TBlock*)malloc(sizeof(TBlock) * width * height);
    for (int i = 0; i < width * height; i++) {
        tetf->blocks[i].n = 0;
    }
    return tetf;
}

void freeTField(TField *tetf) {
    if (tetf) {
        if (tetf->blocks) {
            free(tetf->blocks);
        }
        free(tetf);
    }
}

TGame* start_Game(int width, int height, int size, int count, TBlock *temp_fig) {
    TGame *tetg = (TGame*)malloc(sizeof(TGame));
    tetg->field = build_field(width, height);
    tetg->figures = build_figures(temp_fig, count, size);
    tetg->ticks = TICKS_START;
    tetg->ticks_temp = TICKS_START;
    tetg->score = 0;
    tetg->game = PLAYING;
    return tetg;
}

void freeTGame(TGame *tetg) {
    if (tetg) {
        freeTField(tetg->field);
        freeTFigures(tetg->figures);
        free(tetg);
    }
}

void move_up(TGame *tetg) {
    tetg->figure->y--;
}

void move_down(TGame *tetg) {
    tetg->figure->y++;
}

void move_left(TGame *tetg) {
    tetg->figure->x--;
}

void move_right(TGame *tetg) {
    tetg->figure->x++;
}

int clash(TGame *tetg) {
    TField *tf = tetg->field;
    TFigure *t = tetg->figure;
    for (int i = 0; i < t->size; i++) {
        for (int j = 0; j < t->size; j++) {
            if (t->block[i * t->size + j].n != 0) {
                int fx = t->x + j;
                int fy = t->y + i;
                if (tf->blocks[fy * tf->width + fx].n != 0) {
                    return 1;
                }
                if (fx < 0 || fx >= tf->width || fy < 0 || fy >= tf->height + 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void compound_fig(TGame *tetg) {
    TFigure *tfig = tetg->figure;
    for (int i = 0; i < tfig->size; i++) {
        for (int j = 0; j < tfig->size; j++) {
            if (tfig->block[i * tfig->size + j].n != 0) {
                int fx = tfig->x + j;
                int fy = tfig->y + i;
                tetg->field->blocks[fy * tetg->field->width + fx].n = tfig->block[i * tfig->size + j].n;
            }
        }
    }
}

void tact_game(TGame *tetg) {
    //if (tetg->ticks_temp <= 0) {
        tetg->ticks_temp = tetg->ticks;
        move_down(tetg);
        if (clash(tetg)) {
            move_up(tetg);
            compound_fig(tetg);
            tetg->score += deleteline(tetg);
            drop_fig(tetg);
            if (clash(tetg)) {
                tetg->game = GAMEOVER;
                return;
            }
        }
    //}
    switch(tetg->player->action) {
        case BUT_F:
            TFigure *old = tetg->figure;
            TFigure *new = turn_fig(tetg);
            tetg->figure = new;
            if (clash(tetg)) {
                tetg->figure = old;
                freeTFigure(new);
            } else {
                freeTFigure(old);
            }
            break;
        case BUT_DOWN:
            move_down(tetg);
            if (clash(tetg)) {
                move_up(tetg);
            }
            break;
        case BUT_LEFT:
            move_left(tetg);
            if (clash(tetg)) {
                move_right(tetg);
            }
            break;
        case BUT_RIGHT:
            move_right(tetg);
            if (clash(tetg)) {
                move_left(tetg);
            }
            break;
        default: 
            break;
    }
    tetg->ticks_temp--;
}

int deleteline(TGame *tetg) {
    int count = 0;
    TField *tetf = tetg->field;
    for (int i = tetf->height - 1; i >= 0; i--) {
        while (full_line(tetf, i)) {
            line_down(tetf, i);
            count++;
        }
    }
    return count;
}

int full_line(TField *tetf, int i) {
    for (int j = 0; j < tetf->width; j++) {
        if (tetf->blocks[i * tetf->width +j].n == 0) {
            return 0;
        }
    }
    return 1;
}

void line_down(TField *tetf, int i) {
    if (i == 0) {
        for (int j = 0; j < tetf->width; j++) {
            tetf->blocks[j].n = 0;
        }
    } else {
        for (int k = i; k > 0; k--) {
            for (int j = 0; j < tetf->width; j++) {
                tetf->blocks[k * tetf->width + j].n = tetf->blocks[(k - 1) * tetf->width + j].n;
            }
        }
    }
}

void drop_fig(TGame *tetg) {
    srand(time(NULL));
    TFigure *fig = build_fig(tetg);
    fig->x = tetg->field->width / 2 - fig->size / 2;
    fig->y = 0;
    int nfig = rand() % tetg->figures->count;
    for (int i = 0; i < fig->size; i++) {
        for (int j = 0; j < fig->size; j++) {
            fig->block[i * fig->size + j].n = tetg->figures->blocks[nfig * fig->size * fig->size + i * fig->size + j].n;
        }
    }
    freeTFigure(tetg->figure);
    tetg->figure = fig;
}

TFigure* build_fig(TGame *tetg) {
    TFigure *tetf = (TFigure*)malloc(sizeof(TFigure));
    tetf->x = 0;
    tetf->y = 0;
    tetf->size = tetg->figures->size;
    tetf->block = (TBlock*)malloc(sizeof(TBlock) * tetf->size * tetf->size);
    return tetf;
}

void freeTFigure(TFigure *tetf) {
    if (tetf) {
        if (tetf->block) {
            free(tetf->block);
        }
        free(tetf);
    }
}

TFigure* turn_fig(TGame *tetg) {
    TFigure *new = build_fig(tetg);
    TFigure *old = tetg->figure;
    new->x = old->x;
    new->y = old->y;
    for (int i = 0; i < new->size; i++) {
        for (int j = 0; j < new->size; j++) {
            new->block[i * new->size + j].n = old->block[j * new->size + new->size - 1 - i].n;
        }
    }
    return new;
}

#endif