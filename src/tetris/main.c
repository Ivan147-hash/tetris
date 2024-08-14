#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "tetris.h"

TBlock all_blocks[] = {
    0, 0, 1, 0,
    0, 0, 1, 0,
    0, 0, 1, 0,
    0, 0, 1, 0,

    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    1, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 0, 0, 1,
    0, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 0, 1, 1,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 0, 1, 0,
    0, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,

    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

void print_tet(TGame *tetg, WINDOW *window) {
    TField *tetf = tetg->field;
    TFigure *fig = tetg->figure;
    for (int i = 0; i < tetf->height; i++) {
        for (int j = 0; j < tetf->width; j++) {
            int chr = 1;
            if (tetf->blocks[i * tetf->width + j].n != 0) {
                chr = 2;
            } else {
                int x = j - fig->x;
                int y = i - fig->y;
                if (x >= 0 && x < fig->size && y >= 0 && y < fig->size) {
                    if (fig->block[y * fig->size + x].n != 0) chr = 2;
                }
            }
            // if (chr) printf("#");
            // else printf(" ");
            attron(COLOR_PAIR(chr));
            mvaddch(i, j, ' ');
            attroff(COLOR_PAIR(chr));
        if (chr == 1) {
        wbkgdset(window, COLOR_PAIR(chr));
        mvwprintw(window, i + 1, 2 * j + 1, "%c", ' ');
        mvwprintw(window, i + 1, 2 * j + 2, "%c", ' ');
      } else if (chr == 2) {
        wbkgdset(window, COLOR_PAIR(chr));
        mvwprintw(window, i + 1, 2 * j + 1, "%c", ' ');
        mvwprintw(window, i + 1, 2 * j + 2, "%c", ' ');
      }
        }
    }
}

int main(int argc, char* argv[]) {
    struct  timespec sp_start, sp_end, ts1, ts2 = {0, 0};

    initscr();

    WINDOW *window = newwin(20, 20, 5, 5);

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

    TGame * tetg = start_Game(20, 20, 4, 7, all_blocks);

    // int make_des = rand() % 30;

    TPlayer player;
    player.action = BUT_NON;
    tetg->player = &player;
    drop_fig(tetg);

    while (tetg->game != GAMEOVER) {
        clock_gettime(CLOCK_MONOTONIC, &sp_start);
        // if(make_des <= 0) {
        //     make_des = rand() % 30;
        //     player.action = rand() % 5;
        // }
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
        tact_game(tetg);
        print_tet(tetg, window);
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "Score: %d", tetg->score);
        move(tetg->field->height+1, tetg->field->width+1);
        attroff(COLOR_PAIR(1));
        refresh();
        // make_des--;
        clock_gettime(CLOCK_MONOTONIC, &sp_end);
        if (sp_end.tv_sec - sp_start.tv_sec <= 0 && (ts2.tv_nsec = 330000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)
            nanosleep(&ts2, &ts1);
    }
    freeTGame(tetg);
    wclear(stdscr);
    endwin();
    return 0;
}