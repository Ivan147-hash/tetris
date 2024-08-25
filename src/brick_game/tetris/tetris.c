#include "tetris.h"

#ifndef TETRIS_C
#define TETRIS_C

TBlock all_blocks[] = {
    {0}, {0}, {1}, {0}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {0}, {1},
    {0}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {1},
    {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {0}, {1}, {1},
    {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1}, {0}, {0}, {1}, {1},
    {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {1}, {1},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {1},
    {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {1},
    {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
};

// Создание всех фигур
TFigures *build_figures(TBlock *temp_fig, int count) {
  TFigures *tetfig = (TFigures *)malloc(sizeof(TFigures));
  tetfig->count = count;
  //tetfig->size = size;
  tetfig->blocks = temp_fig;
  return tetfig;
}

// Очищение памяти выделенной под фигуры
void freeTFigures(TFigures *tetfig) {
  if (tetfig) {
    free(tetfig);
  }
}

// Создание одной фигуры
TField *build_field() {
  TField *tetf = (TField *)malloc(sizeof(TField));
  // tetf->width = width;
  // tetf->height = height;
  tetf->blocks = (TBlock *)malloc(sizeof(TBlock) * WIDTH * HIGHT);
  for (int i = 0; i < WIDTH * HIGHT; i++) {
    tetf->blocks[i].n = 0;
  }
  return tetf;
}

// Очищение памяти выделенной под оду фигуру
void freeTField(TField *tetf) {
  if (tetf) {
    if (tetf->blocks) {
      free(tetf->blocks);
    }
    free(tetf);
  }
}

// Создание параметров структуры игры
TGame *start_Game(int count) {
  TGame *tetg = (TGame *)malloc(sizeof(TGame));
  tetg->field = build_field();
  tetg->figure = build_fig();
  tetg->figures = build_figures(all_blocks, count);
  tetg->score = 0;
  tetg->status = START;
  tetg->hard_score = read_record();
  tetg->level = 1;
  return tetg;
}

// Очищение памяти выделенной под структуру параметров
void freeTGame(TGame *tetg) {
  if (tetg) {
    freeTField(tetg->field);
    freeTFigure(tetg->figure);
    freeTFigures(tetg->figures);
    free(tetg);
  }
}

// Движение фигры в вверх
void move_up(TGame *tetg) { tetg->figure->y--; }

// Движение фигры в вниз
void move_down(TGame *tetg) { tetg->figure->y++; }

// Движение фигры вправо
void move_left(TGame *tetg) { tetg->figure->x--; }

// Движение фигры влево
void move_right(TGame *tetg) { tetg->figure->x++; }

// Проверка на столкновение фигуры с объектами
int clash(TGame *tetg) {
  TField *tf = tetg->field;
  TFigure *t = tetg->figure;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (t->block[i * SIZE + j].n != 0) {
        int fx = t->x + j;
        int fy = t->y + i;
        if (tf->blocks[fy * WIDTH + fx].n != 0) return 3;
        if (fx < 0) return 1;
        if (fx >= WIDTH) return 2;
        if (fy < 0 || fy >= HIGHT) return 3;
      }
    }
  }
  return 0;
}

// Соединение упавшего блока с общей конструкцией
int compound_fig(TGame *tetg) {
  TFigure *tfig = tetg->figure;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (tfig->block[i * SIZE + j].n != 0) {
        int fx = tfig->x + j;
        int fy = tfig->y + i;
        tetg->field->blocks[fy * WIDTH + fx].n =
            tfig->block[i * SIZE + j].n;
      }
    }
  }
  return 1;
}

// Игровой такт
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
  }
}

// Удаление заполненных линий и начисление очков
int deleteline(TGame *tetg) {
  int count = 0;
  TField *tetf = tetg->field;
  for (int i = HIGHT - 1; i >= 0; i--) {
    while (full_line(tetf, i)) {
      line_down(tetf, i);
      count++;
    }
  }
  if (count == 1)
    count = 100;
  else if (count == 2)
    count = 300;
  else if (count == 3)
    count = 700;
  else if (count >= 4)
    count = 1500;
  return count;
}

// Проверка на заполненность линии
int full_line(TField *tetf, int i) {
  for (int j = 0; j < WIDTH; j++) {
    if (tetf->blocks[i * WIDTH + j].n == 0) {
      return 0;
    }
  }
  return 1;
}

// Смещение всех блоков вниз
void line_down(TField *tetf, int i) {
  if (i == 0) {
    for (int j = 0; j < WIDTH; j++) {
      tetf->blocks[j].n = 0;
    }
  } else {
    for (int k = i; k > 0; k--) {
      for (int j = 0; j < WIDTH; j++) {
        tetf->blocks[k * WIDTH + j].n =
            tetf->blocks[(k - 1) * WIDTH + j].n;
      }
    }
  }
}

// Выпадение новой фигуры
void drop_fig(TGame *tetg) {
  srand(time(NULL));
  TFigure *fig = build_fig();
  fig->x = WIDTH / 2 - SIZE / 2;
  fig->y = 1;
  if (tetg->status == 1)
    tetg->fig = rand() % tetg->figures->count;
  else
    tetg->fig = tetg->next_fig;
  int f = tetg->fig;
  if (tetg->fig == 1) fig->x -= 1;
  tetg->next_fig = rand() % tetg->figures->count;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      fig->block[i * SIZE + j].n = tetg->figures->blocks[f * SIZE * SIZE + i * SIZE + j].n;
    }
  }
  freeTFigure(tetg->figure);

  tetg->figure = fig;
}

// Создание фигуры
TFigure *build_fig() {
  TFigure *tetf = (TFigure *)malloc(sizeof(TFigure));
  tetf->x = 0;
  tetf->y = 0;
  //tetf->size = SIZE;
  tetf->block = (TBlock *)malloc(sizeof(TBlock) * SIZE * SIZE);
  return tetf;
}

// Очищение памяти выделенной под фигуру
void freeTFigure(TFigure *tetf) {
  if (tetf) {
    if (tetf->block) {
      free(tetf->block);
    }
    free(tetf);
  }
}

// Поворот фигуры
TFigure *turn_fig(TGame *tetg) {
  TFigure *new = build_fig();
  TFigure *old = tetg->figure;
  new->x = old->x;
  new->y = old->y;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      new->block[i * SIZE + j].n = old->block[j * SIZE + SIZE - 1 - i].n;
    }
  }
  return new;
}

// Чтение рекорда из файла
int read_record() {
  int hard = 2;
  FILE *fp = fopen("record.txt", "r");
  if (fp != NULL) {
    fscanf(fp, "%d", &hard);
    fclose(fp);
  }
  return hard;
}

// Запись нового рекорда
void write_record(TGame *tetg) {
  if (tetg->hard_score < tetg->score) {
    FILE *fp = fopen("record.txt", "w");
    if (fp != NULL) {
      fprintf(fp, "%d", tetg->score);
      fclose(fp);
    }
  }
}

// Повышение скорости игры
int level_up(TGame *tetg) {
  int speed = 500;
  tetg->level = tetg->score / 600 + 1;
  speed = 550 - tetg->level * 50;
  return speed;
}

// Действия при нажатии клавиши вверх
void key_up(TGame *tetg) {
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
  tetg->status = TURN;
}

// Совершение действия игроком
void action_play(TGame *tetg, int chr) {
  switch (chr) {
    case KEY_UP:
      key_up(tetg);
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
      if (tetg->status == PAUSE)
        tetg->status = PLAYING;
      else {
        tetg->status = PAUSE;
      }
      break;
    default:
      if (tetg->status != GAMEOVER && tetg->status != PAUSE)
        tetg->status = DOWN;
      break;
  }
}

#endif