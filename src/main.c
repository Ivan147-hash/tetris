#include "brick_game/tetris/tetris.h"
#include "gui/cli/front.h"

int main() {
  struct timespec ts1;

  init_ncurses();

  Windows *WIN = init_win();

  TGame *tetg = start_Game();

  clock_gettime(CLOCK_MONOTONIC, &ts1);
  long last_time = ts1.tv_sec * 1000 + ts1.tv_nsec / 1000000;
  int move_interval = 500;

  while (tetg->status != GAMEOVER) {
    if (tetg->status == START) drop_fig(tetg);
    print_front(tetg, WIN);

    int chr = getch();
    action_play(tetg, chr);

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
  if (chr) freeTGame(tetg);
  free(WIN);
  wclear(stdscr);
  endwin();
  return 0;
}