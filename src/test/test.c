#include <check.h>
#include <stdio.h>

#include "../brick_game/tetris/tetris.h"

START_TEST(tetris_test_1) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  ck_assert_int_eq(tetg->status, START);
  action_play(tetg, KEY_UP);
  ck_assert_int_eq(tetg->status, TURN);
  action_play(tetg, KEY_DOWN);
  ck_assert_int_eq(tetg->status, DOWN);
  action_play(tetg, KEY_LEFT);
  ck_assert_int_eq(tetg->status, LEFT);
  action_play(tetg, KEY_RIGHT);
  ck_assert_int_eq(tetg->status, RIGHT);
  action_play(tetg, 'a');
  ck_assert_int_eq(tetg->status, DOWN);
  action_play(tetg, ' ');
  ck_assert_int_eq(tetg->status, PAUSE);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}
END_TEST

Suite *test_status() {
  Suite *s = suite_create("test_status");
  TCase *tc = tcase_create("test_status");
  tcase_add_test(tc, tetris_test_1);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tetris_test_2) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  int flag = level_up(tetg);
  ck_assert_int_eq(flag, 500);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}

END_TEST

Suite *test_level_up() {
  Suite *s = suite_create("test_level_up");
  TCase *tc = tcase_create("test_level_up");
  tcase_add_test(tc, tetris_test_2);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tetris_test_3) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  tetg->figure->y = 18;
  compound_fig(tetg);
  ck_assert_int_eq(tetg->field->blocks[20 * 10 + 5].n, 1);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}

END_TEST

Suite *test_compound() {
  Suite *s = suite_create("test_compound");
  TCase *tc = tcase_create("test_compound");
  tcase_add_test(tc, tetris_test_3);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tetris_test_4) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  int i = HIGHT - 1;
  for (int j = 0; j < WIDTH; j++) {
    tetg->field->blocks[i * WIDTH + j].n = 0;
  }
  int flag = deleteline(tetg);
  ck_assert_int_eq(flag, 0);

  for (int j = 0; j < WIDTH; j++) {
    tetg->field->blocks[i * WIDTH + j].n = 1;
  }
  flag = deleteline(tetg);
  ck_assert_int_eq(flag, 100);
  for (int x = i; x > i - 2; x--) {
    for (int j = 0; j < WIDTH; j++) {
      tetg->field->blocks[x * WIDTH + j].n = 1;
    }
  }
  flag = deleteline(tetg);
  ck_assert_int_eq(flag, 300);
  for (int x = i; x > i - 3; x--) {
    for (int j = 0; j < WIDTH; j++) {
      tetg->field->blocks[x * WIDTH + j].n = 1;
    }
  }
  flag = deleteline(tetg);
  ck_assert_int_eq(flag, 700);
  for (int x = i; x > i - 4; x--) {
    for (int j = 0; j < WIDTH; j++) {
      tetg->field->blocks[x * WIDTH + j].n = 1;
    }
  }
  flag = deleteline(tetg);
  ck_assert_int_eq(flag, 1500);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}

END_TEST

Suite *test_score() {
  Suite *s = suite_create("test_score");
  TCase *tc = tcase_create("test_score");
  tcase_add_test(tc, tetris_test_4);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tetris_test_5) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  tetg->fig = 2;
  int i = HIGHT - 1;
  for (int j = 0; j < WIDTH; j++) {
    tetg->field->blocks[i * WIDTH + j].n = 1;
  }
  tetg->figure->y = 22;
  tact_game(tetg);
  ck_assert_int_eq(tetg->score, 100);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}

END_TEST

Suite *test_tact_game_1() {
  Suite *s = suite_create("test_tact_game_1");
  TCase *tc = tcase_create("test_tact_game_1");
  tcase_add_test(tc, tetris_test_5);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tetris_test_6) {
  TGame *tetg = start_Game();
  drop_fig(tetg);
  tetg->fig = 2;
  tetg->figure->x = -3;
  tetg->figure->y = 2;
  tact_game(tetg);
  ck_assert_int_eq(tetg->score, 0);
  tetg->status = GAMEOVER;
  freeTGame(tetg);
}

END_TEST

Suite *test_tact_game_2() {
  Suite *s = suite_create("test_tact_game_2");
  TCase *tc = tcase_create("test_tact_game_2");
  tcase_add_test(tc, tetris_test_6);
  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int fail = 0;
  Suite *s[] = {test_status(), test_level_up(),    test_compound(),
                test_score(),  test_tact_game_1(), test_tact_game_2(),
                NULL};
  for (int i = 0; s[i] != NULL; i++) {
    SRunner *runner = srunner_create(s[i]);
    srunner_run_all(runner, CK_NORMAL);
    fail += srunner_ntests_failed(runner);
    srunner_free(runner);
  }
  printf("FAIL: %d\n", fail);
  return 0;
}