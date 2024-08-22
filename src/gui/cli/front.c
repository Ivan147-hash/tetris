#include "front.h"

void print_tet(TGame *tetg, WINDOW *board) {
    TField *tetf = tetg->field;
    TFigure *fig = tetg->figure;
    int color = 1;
    int col_fig;
    if (tetg->status == START) color = 4;
    werase(board);
    wbkgd(board, COLOR_PAIR(3));
    for (int i = 0; i < tetf->height + 5; i++) {
        for (int j = 0; j < tetf->width; j++) {
            if (i >= 5) {
                wbkgdset(board, COLOR_PAIR(color));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            }
            if (tetf->blocks[i * tetf->width + j].n == 1) {
                wbkgdset(board, COLOR_PAIR(4));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            } 
            else
                {int x = j - fig->x;
                int y = i - fig->y;
                if (x >= 0 && x < fig->size && y >= 0 && y < fig->size) {
                    if (fig->block[y * fig->size + x].n != 0) {
                        wbkgdset(board, COLOR_PAIR(2));
                        mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                        mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
                    }
                }
            }
             if (i < 5) {
                wbkgdset(board, COLOR_PAIR(3));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            }
        }
    }
     wrefresh(board);
}

void print_rec(TGame *tetg, WINDOW *record) {
    werase(record);
  int score = tetg->score;
  int high = tetg->hard_score;
  int speed = tetg->level;
  if (score > high) high = score;
  wbkgd(record, COLOR_PAIR(1));
  wattron(record, COLOR_PAIR(1));
  mvwprintw(record, 1, 1, "SCORE:");
  mvwprintw(record, 2, 1, "%d", score);
  mvwprintw(record, 4, 1, "RECORD:");
  mvwprintw(record, 5, 1, "%d", high);
  mvwprintw(record, 7, 1, "LEVEL:");
  mvwprintw(record, 8, 1, "%d", speed);
  wattroff(record, COLOR_PAIR(1));
  wrefresh(record);
}

void print_fig(TGame *tetg, WINDOW *next_fig) {
    werase(next_fig);
    wbkgd(next_fig, COLOR_PAIR(1));
    for (int row = 0; row < tetg->figure->size; row++) {
        for (int col = 0; col < tetg->figure->size; col++) {
            if (tetg->figures->blocks[tetg->next_fig * tetg->figure->size * tetg->figure->size + row * tetg->figure->size + col].n == 0) {
                wbkgdset(next_fig, COLOR_PAIR(1));
                mvwprintw(next_fig, row + 1, 2 * col + 1, "%c", ' ');
                mvwprintw(next_fig, row + 1, 2 * col + 2, "%c", ' ');
            } else if (tetg->figures->blocks[tetg->next_fig * tetg->figure->size * tetg->figure->size + row * tetg->figure->size + col].n == 1) {
                wbkgdset(next_fig, COLOR_PAIR(2));
                mvwprintw(next_fig, row + 1, 2 * col + 1, "%c", ' ');
                mvwprintw(next_fig, row + 1, 2 * col + 2, "%c", ' ');
            }
        }
    }
    wbkgdset(next_fig, COLOR_PAIR(1));
    mvwprintw(next_fig, 0, 1, "NEXT FIGURE");
    wattroff(next_fig, COLOR_PAIR(1));
    wrefresh(next_fig);
}

void print_pause(TGame *tetg, WINDOW *board) {
  wbkgdset(board, COLOR_PAIR(1));
  mvwprintw(board, 13, 8, "PAUSE");
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

void print_game_over(TGame *tetg, WINDOW *board) {
  wbkgdset(board, COLOR_PAIR(1));
  mvwprintw(board, 13, 6, "GAME OVER!");
  mvwprintw(board, 14, 6, "SCORE: %d", tetg->score);
  mvwprintw(board, 15, 4, "HARD SCORE: %d", tetg->hard_score);
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

void print_start(TGame *tetg, WINDOW *board) {
    getch();
  print_tet(tetg, board);
  curs_set(0);
  wbkgdset(board, COLOR_PAIR(4));
  mvwprintw(board, 8, 7, "S21_TETRIS");

  mvwprintw(board, 10, 3, "UP - rotation");
  mvwprintw(board, 11, 3, "DOWN - descent");
  mvwprintw(board, 12, 3, "LEFT - move left");
  mvwprintw(board, 13, 3, "RIGHT - move right");
  mvwprintw(board, 14, 3, "SPACE - pause");
  mvwprintw(board, 15, 3, "Q - exit");

  mvwprintw(board, 22, 5, "PRESS ANY KEY");
  mvwprintw(board, 23, 8, "TO START");
  mvwprintw(board, 25, 5, "s21_rossiesh");
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

void print_front(TGame *tetg, Windows *win) {
    if (tetg->status == START) print_start(tetg, win->board);
    print_tet(tetg,  win->board);
    print_rec(tetg,  win->record);
    print_fig(tetg,  win->next_fig);
    if (tetg->status == PAUSE) {
        print_pause(tetg,  win->board);
        tetg->status = PLAYING;
    }
}