#include "front.h"

// Отрисовка игрового поля
void print_tet(TGame *tetg, WINDOW *board) {
  TField *tetf = tetg->field;
  TFigure *fig = tetg->figure;
  int n_fig = tetg->fig + 1;
  int color = 8;
  if (tetg->status == START) color = 4;
  werase(board);
  wbkgd(board, COLOR_PAIR(3));
  wattron(board, A_BOLD);
  for (int i = 0; i < HIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (i >= 5) {
        wbkgdset(board, COLOR_PAIR(color));
        mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
        mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
      }
      if (tetf->blocks[i * WIDTH + j].n == 1) {
        wbkgdset(board, COLOR_PAIR(4));
        mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
        mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
      } else {
        print_figure(fig, i, j, board, n_fig);
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

// Отрисовка летящей фигуры
void print_figure(TFigure *fig, int i, int j, WINDOW *board, int n_fig) {
  int x = j - fig->x;
  int y = i - fig->y;
  if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
    if (fig->block[y * SIZE + x].n != 0) {
      if (n_fig != 1) wbkgdset(board, COLOR_PAIR(n_fig)|A_REVERSE);
      else wbkgdset(board, COLOR_PAIR(n_fig));
      mvwprintw(board, i + 1, 2 * j + 1, "%c", ' ');
      mvwprintw(board, i + 1, 2 * j + 2, "%c", ' ');
    }
  }
}

// Отрисовка поля с информацией
void print_rec(TGame *tetg, WINDOW *record) {
  wattron(record, A_BOLD);
  werase(record);
  int score = tetg->score;
  int high = tetg->hard_score;
  int speed = tetg->level;
  if (score > high) high = score;
  wbkgd(record, COLOR_PAIR(8));
  wattron(record, COLOR_PAIR(8));
  mvwprintw(record, 1, 1, "SCORE:");
  mvwprintw(record, 2, 1, "%d", score);
  mvwprintw(record, 4, 1, "RECORD:");
  mvwprintw(record, 5, 1, "%d", high);
  mvwprintw(record, 7, 1, "LEVEL:");
  mvwprintw(record, 8, 1, "%d", speed);
  wattroff(record, COLOR_PAIR(1));
  wrefresh(record);
}

// Отрисовка поля со следующей фигурой
void print_fig(TGame *tetg, WINDOW *next_fig, int n_fig) {
  wattron(next_fig, A_BOLD);
  int fig = tetg->next_fig;
  int next = tetg->next_fig;
  werase(next_fig);
  wbkgd(next_fig, COLOR_PAIR(8));
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      if (tetg->figures->blocks[fig * SIZE * SIZE + row * SIZE + col].n == 0) {
        wbkgdset(next_fig, COLOR_PAIR(8));
        mvwprintw(next_fig, row + 1, 2 * col + 1, "%c", ' ');
        mvwprintw(next_fig, row + 1, 2 * col + 2, "%c", ' ');
      } else if (tetg->figures->blocks[next * SIZE * SIZE + row * SIZE + col]
                     .n == 1) {
        wbkgdset(next_fig, COLOR_PAIR(n_fig)|A_REVERSE);
        mvwprintw(next_fig, row + 1, 2 * col + 1, "%c", ' ');
        mvwprintw(next_fig, row + 1, 2 * col + 2, "%c", ' ');
      }
    }
  }
  wbkgdset(next_fig, COLOR_PAIR(8));
  mvwprintw(next_fig, 0, 1, "NEXT FIGURE");
  wattroff(next_fig, COLOR_PAIR(8));
  wrefresh(next_fig);
}

// Отрисовка поля паузы
void print_pause(WINDOW *board) {
  wattron(board, A_BOLD);
  wbkgdset(board, COLOR_PAIR(1));
  mvwprintw(board, 14, 9, "PAUSE");
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

// Отрисовка поля окончания игры
void print_game_over(TGame *tetg, WINDOW *board) {
  wattron(board, A_BOLD);
  int score = tetg->hard_score;
  if (tetg->hard_score < tetg->score) score = tetg->score;
  wbkgdset(board, COLOR_PAIR(1));
  mvwprintw(board, 13, 6, "GAME OVER!");
  mvwprintw(board, 14, 6, "SCORE: %d", tetg->score);
  mvwprintw(board, 15, 4, "HARD SCORE: %d", score);
  wrefresh(board);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

// Отрисовка поля начала игры
void print_start(TGame *tetg, WINDOW *board) {
  getch();
  print_tet(tetg, board);
  wattron(board, A_BOLD);
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

// Отрисовка всех полей игры
void print_front(TGame *tetg, Windows *win) {
  if (tetg->status == START) print_start(tetg, win->board);
  print_tet(tetg, win->board);
  print_rec(tetg, win->record);
  print_fig(tetg, win->next_fig, tetg->next_fig + 1);
  if (tetg->status == PAUSE) {
    print_pause(win->board);
    tetg->status = PLAYING;
  }
}

// Инициализация библиотеки ncurses
void init_ncurses() {
  initscr();
  start_color();

    init_pair(1,  COLOR_YELLOW,     COLOR_YELLOW);
    init_pair(2,  COLOR_GREEN,   COLOR_GREEN);
    init_pair(3,  COLOR_BLACK,  COLOR_BLACK);
    init_pair(4,  COLOR_BLUE,    COLOR_BLUE);
    init_pair(5,  COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(6,  COLOR_CYAN,    COLOR_CYAN);
    init_pair(7,  COLOR_RED,    COLOR_RED);
    init_pair(8,  COLOR_WHITE,   COLOR_WHITE);

  cbreak();
  keypad(stdscr, TRUE);
  timeout(0);
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
}

// Создание окон игры
Windows *init_win() {
  Windows *win = (Windows *)malloc(sizeof(Windows));
  if (win != NULL) {
    win->board = newwin(27, 22, 5, 5);
    win->next_fig = newwin(6, 14, 25, 30);
    win->record = newwin(12, 14, 11, 30);
  }
  return win;
}