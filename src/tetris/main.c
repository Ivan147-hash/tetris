// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <ncurses.h>

#include "tetris.h"

TBlock all_blocks[] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 1, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    0, 0, 0, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

// void color_block(TField *tetf, TFigure *fig, int i, int j) {
//     for (int i = 5; i < tetf->height + 5; i++) {
//         for (int j = 0; j < tetf->width; j++) {
//             int x = j - fig->x;
//             int y = i - fig->y;
//             if (x >= 0 && x < fig->size && y >= 0 && y < fig->size) {
//                 if (fig->block[y * fig->size + x].n != 0) {
//                     tetf->blocks[i * tetf->width + j].n = 1;
//                 }
//             }
//         }
//     }
// }

void print_tet(TGame *tetg, WINDOW *board) {
    TField *tetf = tetg->field;
    TFigure *fig = tetg->figure;
    werase(board);
    wbkgd(board, COLOR_PAIR(3));
    for (int i = 0; i < tetf->height + 5; i++) {
        for (int j = 0; j < tetf->width; j++) {
            int flag = 0;
            if (i >= 5) {
                wbkgdset(board, COLOR_PAIR(1));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            }
            if (tetf->blocks[i * tetf->width + j].n == 1) {
                wbkgdset(board, COLOR_PAIR(2));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
                flag = 1;
            } 
            else // if (tetf->blocks[i * tetf->width + j].n == 2) {
                {int x = j - fig->x;
                int y = i - fig->y;
                if (x >= 0 && x < fig->size && y >= 0 && y < fig->size) {
                    if (fig->block[y * fig->size + x].n != 0) {
                        wbkgdset(board, COLOR_PAIR(2));
                        mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                        mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
                    }
                }
                flag = 1;
            }
             if (i < 5) {
                wbkgdset(board, COLOR_PAIR(3));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            }
            // if (!flag) {
            //     wbkgdset(board, COLOR_PAIR(1));
            //     mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
            //     mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
            // }
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
    mvprintw(11, 31, "NEXT FIGURE");
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
  //print_tet(tetg, board);
  wbkgdset(board, COLOR_PAIR(1));
  mvwprintw(board, 13, 8, "S21_TETRIS");
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

int main(int argc, char* argv[]) {
    struct  timespec sp_start, sp_end, ts1, ts2 = {0, 0};

    initscr();

    WINDOW *start = newwin(26, 22, 5, 5);
    WINDOW *board = newwin(26, 22, 5, 5);
    WINDOW *record = newwin(12, 12, 19, 30);
    WINDOW *next_fig = newwin(6, 12, 11, 30);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    cbreak();
    keypad(stdscr, TRUE);
    timeout(0);
    //curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    TGame * tetg = start_Game(10, 25, 5, 7, all_blocks);

    TPlayer player;
    player.action = BUT_NON;
    tetg->player = &player;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    long last_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;
    int move_interval = 500;

    drop_fig(tetg);

     print_start(tetg, start);

    while (tetg->status != GAMEOVER) {
        // if (tetg->status == START) print_start(tetg, start);
        // else {
        print_tet(tetg, board);
        if (tetg->status == START) print_start(tetg, start);
        print_rec(tetg, record);
        print_fig(tetg, next_fig);
        // }
        //clock_gettime(CLOCK_MONOTONIC, &sp_start);
        // clock_gettime(CLOCK_MONOTONIC, &ts1);
        // long current_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;

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
                print_pause(tetg, board);
            }
            break;
        default: 
            if (tetg->status != GAMEOVER && tetg->status != PAUSE) tetg->status = DOWN;
            break;
    }
        move_interval = level_up(tetg);
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        long current_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;
        if (current_time - last_time >= move_interval) {
            tact_game(tetg);
            // else print_pause(tetg, board);
            // print_tet(tetg, board);
            // print_rec(tetg, record);
            // print_fig(tetg, next_fig);
            //attron(COLOR_PAIR(1));
            //move(tetg->field->height+1, tetg->field->width+1);
            //attroff(COLOR_PAIR(1));

            // refresh();

            // clock_gettime(CLOCK_MONOTONIC, &sp_end);
            // if (sp_end.tv_sec - sp_start.tv_sec <= 0 && (ts2.tv_nsec = 930000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)
            //     nanosleep(&ts2, &ts1);
            last_time = current_time;
        }
    }
    print_game_over(tetg, board);
    int chr = getch();
    write_record(tetg);
    if (chr) freeTGame(tetg);
    wclear(stdscr);
    endwin();
    return 0;
}