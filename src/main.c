#include "brick_game/tetris/tetris.h"
#include "gui/cli/front.h"

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

int main(int argc, char* argv[]) {
    struct  timespec sp_start, sp_end, ts1, ts2 = {0, 0};

    initscr();

    Windows *WIN = init_win();
    // WINDOW *board = newwin(26, 22, 5, 5);
    // WINDOW *record = newwin(12, 14, 19, 30);
    // WINDOW *next_fig = newwin(6, 14, 11, 30);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);

    cbreak();
    keypad(stdscr, TRUE);
    timeout(0);
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

    while (tetg->status != GAMEOVER) {
        print_front(tetg, WIN);
        // if (tetg->status == START) print_start(tetg, board);
        // print_tet(tetg, board);
        // print_rec(tetg, record);
        // print_fig(tetg, next_fig);
        // if (tetg->status == PAUSE) {
        //     print_pause(tetg, board);
        //     tetg->status = PLAYING;
        // }

        action_play(tetg, WIN->board);

        move_interval = level_up(tetg);
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        long current_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;
        if (current_time - last_time >= move_interval) {
            tact_game(tetg);
            last_time = current_time;
        }
    }
    print_game_over(tetg, WIN->board);
    int chr = getch();
    write_record(tetg);
    if (chr) freeTGame(tetg, WIN);
    wclear(stdscr);
    endwin();
    return 0;
}