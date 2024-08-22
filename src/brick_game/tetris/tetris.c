#include "tetris.h"

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
    tetg->score = 0;
    tetg->status = START;
    tetg->hard_score = read_record();
    tetg->level = 1;
    return tetg;
}

void freeTGame(TGame *tetg, Windows *win) {
    if (tetg) {
        freeTField(tetg->field);
        freeTFigures(tetg->figures);
        free(tetg);
        freeWindows(win);
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
                if (tf->blocks[fy * tf->width + fx].n != 0) return 3;
                if(fx < 0) return 1;
                if(fx >= tf->width) return 2;
                if (fy < 0 || fy >= tf->height) return 3;
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
        move_down(tetg);
        if (clash(tetg) && tetg->figure->y <= 5) {
            tetg->status = GAMEOVER;
            return;
        }
        if (clash(tetg)) {
            move_up(tetg);
            compound_fig(tetg);
            tetg->score += deleteline(tetg);
            drop_fig(tetg);
            if (clash(tetg)) {
                tetg->status = GAMEOVER;
                return;
            }
        }
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
    if (count == 1) count = 100;
    else if (count == 2) count = 300;
    else if (count == 3) count = 700;
    else if (count >= 4) count = 1500;
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

void drop_fig(TGame *tetg) { // 1, 7, 8, 10
    srand(time(NULL));
    TFigure *fig = build_fig(tetg);
    fig->x = tetg->field->width / 2 - fig->size / 2;
    fig->y = 1;
    if (tetg->status == 1) tetg->fig = rand() % tetg->figures->count;
    else if (tetg->status != 1) tetg->fig = tetg->next_fig;
    if (tetg->fig == 1) fig->x -= 1;
    tetg->next_fig = rand() % tetg->figures->count;
    for (int i = 0; i < fig->size; i++) {
        for (int j = 0; j < fig->size; j++) {
            fig->block[i * fig->size + j].n = tetg->figures->blocks[tetg->fig * fig->size * fig->size + i * fig->size + j].n;
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

int read_record() {
    int hard = 2;
    FILE *fp = fopen("record.txt", "r");
    if(fp != NULL)
    {
        fscanf(fp, "%d", &hard);
        fclose(fp);
    }
    return hard;
}

void write_record(TGame *tetg) {
    if (tetg->hard_score < tetg->score) {
        FILE *fp = fopen("record.txt", "w");
        if(fp != NULL)
        {
            fprintf(fp, "%d", tetg->score);
            fclose(fp);
        }
    }
}

int level_up(TGame *tetg) {
    int speed = 500;
    tetg->level = tetg->score / 600 + 1;
    speed = 550 - tetg->level * 50;
    return speed;
}

void action_play(TGame *tetg, WINDOW *board) {
    int chr = getch();
        switch(chr) {
        case KEY_UP:
            TFigure *old = tetg->figure;
            TFigure *new = turn_fig(tetg);
            tetg->figure = new;
            if (clash(tetg) == 1) move_right(tetg);
            if (clash(tetg) == 2) move_left(tetg);
            if (clash(tetg) || tetg->fig == 1) {
                tetg->figure = old;
                freeTFigure(new);
            } else {
                freeTFigure(old);
            }
            tetg->status = UP;
            break;
        case KEY_DOWN:
            move_down(tetg);
            if (clash(tetg)) {
                move_up(tetg);
            }
            tetg->status = DOWN;
            break;
        case KEY_LEFT:
            move_left(tetg);
            if (clash(tetg)) {
                move_right(tetg);
            }
            tetg->status = LEFT;
            break;
        case KEY_RIGHT:
            move_right(tetg);
            if (clash(tetg)) {
                move_left(tetg);
            }
            tetg->status = RIGHT;
            break;
        case 'q':
            tetg->status = GAMEOVER;
            break;
        case ' ':
            if (tetg->status == PAUSE) tetg->status = PLAYING;
            else {
                tetg->status = PAUSE;
            }
            break;
        default: 
            if (tetg->status != GAMEOVER && tetg->status != PAUSE) tetg->status = DOWN;
            break;
    }
}

Windows* init_win() {
    Windows *win = (Windows *)malloc(sizeof(Windows));
    if (win != NULL) {
        win->board = newwin(26, 22, 5, 5);
        win->next_fig = newwin(6, 14, 11, 30);;
        win->record = newwin(12, 14, 19, 30);
    }
}

void freeWindows(Windows *win) {
    if (win) {
        free(win);
    }
}


#endif