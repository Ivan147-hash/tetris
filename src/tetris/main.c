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
    0, 0, 1, 1, 0,
    0, 0, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

    1, 0, 0, 0, 0,
    1, 1, 1, 0, 0,
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

    1, 1, 0, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

void print_tet(TGame *tetg, WINDOW *board) {
    TField *tetf = tetg->field;
    TFigure *fig = tetg->figure;
    werase(board);
    wbkgd(board, COLOR_PAIR(1));
    for (int i = 0; i < tetf->height; i++) {
        for (int j = 0; j < tetf->width; j++) {
            int chr = 1;
            if (tetf->blocks[i * tetf->width + j].n != 0) {
                wbkgdset(board, COLOR_PAIR(2));
                mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
                //chr = 2;
            } else {
                int x = j - fig->x;
                int y = i - fig->y;
                if (x >= 0 && x < fig->size && y >= 0 && y < fig->size) {
                    if (fig->block[y * fig->size + x].n != 0) {
                        wbkgdset(board, COLOR_PAIR(2));
                        mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
                        mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
                        //chr = 2;
                    }
                }
            }
            // if (chr) printf("#");
            // else printf(" ");

            // attron(COLOR_PAIR(chr));
            // mvaddch(i, j, ' ');
            // attroff(COLOR_PAIR(chr));

    //     if (chr == 1) {
    //     wbkgdset(board, COLOR_PAIR(1));
    //     mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
    //     mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
    //   } else if (chr == 2) {
    //     wbkgdset(board, COLOR_PAIR(2));
    //     mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
    //     mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
    //   }
        }
    }
     wrefresh(board);
}

void print_rec(TGame *tetg, WINDOW *record) {
    werase(record);
  int score = tetg->score;
  int high = tetg->score;
//   int speed;
  wbkgd(record, COLOR_PAIR(1));
  wattron(record, COLOR_PAIR(2));
  mvwprintw(record, 1, 1, "SCORE:");
  mvwprintw(record, 2, 1, "%d", score);
  mvwprintw(record, 4, 1, "RECORD:");
  mvwprintw(record, 5, 1, "%d", high);
//   mvwprintw(record, 7, 1, "SPEED:");
//   mvwprintw(record, 8, 1, "%d", speed);
  wattroff(record, COLOR_PAIR(2));
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
    wrefresh(next_fig);
}

int main(int argc, char* argv[]) {
    struct  timespec sp_start, sp_end, ts1, ts2 = {0, 0};

    initscr();

    WINDOW *board = newwin(21, 20, 0, 0);
    WINDOW *record = newwin(11, 12, 9, 30);
    WINDOW *next_fig = newwin(6, 12, 0, 30);

    start_color();
    init_pair(1, COLOR_RED, COLOR_CYAN);
    init_pair(2, COLOR_MAGENTA, COLOR_MAGENTA);
    cbreak();
    keypad(stdscr, TRUE);
    timeout(0);
    //curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    TGame * tetg = start_Game(10, 20, 5, 7, all_blocks);

    // int make_des = rand() % 30;

    TPlayer player;
    player.action = BUT_NON;
    tetg->player = &player;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    long last_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;
    int move_interval = 400;

    drop_fig(tetg);

    while (tetg->game != GAMEOVER) {
        tetg->status = 2;
        //clock_gettime(CLOCK_MONOTONIC, &sp_start);
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        long current_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;

        int chr = getch();
        switch (chr)
        {
        case KEY_LEFT:
            player.action = BUT_LEFT;
            break;
        case KEY_RIGHT:
            player.action = BUT_RIGHT;
            break;
        case KEY_DOWN:
            player.action = BUT_DOWN;
            break;
        case ' ':
            player.action = BUT_F;
            break;
        case 'q':
            tetg->game = GAMEOVER;
            break;
        
        default:
            player.action = BUT_NON;
            break;
        }

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

        // int chr = getch();
        // switch (chr)
        // {
        // case KEY_LEFT:
        //     move_left(tetg);
        //     player.action = BUT_LEFT;
        //     break;
        // case KEY_RIGHT:
        //     move_right(tetg);
        //     player.action = BUT_RIGHT;
        //     break;
        // case KEY_DOWN:
        //     move_down(tetg);
        //     player.action = BUT_DOWN;
        //     break;
        // case ' ':
        //     TFigure *old = tetg->figure;
        //     TFigure *new = turn_fig(tetg);
        //     tetg->figure = new;
        //     if (clash(tetg)) {
        //         tetg->figure = old;
        //         freeTFigure(new);
        //     } else {
        //         freeTFigure(old);
        //     }
        //     player.action = BUT_F;
        //     break;
        // case 'q':
        //     tetg->game = GAMEOVER;
        //     break;
        
        // default:
        //     player.action = BUT_NON;
        //     break;
        // }
        
        if (current_time - last_time >= move_interval) {

        // if(make_des <= 0) {
        //     make_des = rand() % 30;
        //     player.action = rand() % 5;
        // }

        // int chr = getch();
        // switch (chr)
        // {
        // case KEY_LEFT:
        //     player.action = BUT_LEFT;
        //     break;
        // case KEY_RIGHT:
        //     player.action = BUT_RIGHT;
        //     break;
        // case KEY_DOWN:
        //     player.action = BUT_DOWN;
        //     break;
        // case ' ':
        //     player.action = BUT_F;
        //     break;
        // case 'q':
        //     tetg->game = GAMEOVER;
        //     break;
        
        // default:
        //     player.action = BUT_NON;
        //     break;
        // }
        tact_game(tetg);
        print_tet(tetg, board);
        print_rec(tetg, record);
        print_fig(tetg, next_fig);
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "Score: %d", tetg->score);
        move(tetg->field->height+1, tetg->field->width+1);
        attroff(COLOR_PAIR(1));
        refresh();
        // make_des--;
        // clock_gettime(CLOCK_MONOTONIC, &sp_end);
        // if (sp_end.tv_sec - sp_start.tv_sec <= 0 && (ts2.tv_nsec = 930000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)
        //     nanosleep(&ts2, &ts1);
    last_time = current_time;
    }
    }
    freeTGame(tetg);
    wclear(stdscr);
    endwin();
    return 0;
}