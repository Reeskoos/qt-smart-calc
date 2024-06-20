#include <check.h>
#include <math.h>
#include <stdlib.h>

#define EPSILON 1e-7
typedef long double ld;

#include "../src/calc_logic/bank_calc/include/s21_credit_calc.h"
#include "../src/calc_logic/bank_calc/include/s21_deposit_calc.h"
#include "../src/calc_logic/s21_calc.h"
#include "../src/calc_logic/translator/include/translator.h"

START_TEST(test_ok1) {
  const char *expr = "2+2*3/4^5";
  ld result = s21_smart_calc(expr);
  ld expected = 2.0 + 2.0 * 3.0 / powl(4.0, 5.0);
  ck_assert_double_eq_tol(result, expected, EPSILON);
}
END_TEST

START_TEST(test_ok2) {
  const char *expr = "2+(2*3/4)*cos(5)";
  ld result = s21_smart_calc(expr);
  ld expected = 2.0 + (2.0 * 3.0 / 4.0) * cos(5.0);
  ck_assert_double_eq_tol(result, expected, EPSILON);
}
END_TEST

START_TEST(test_ok3) {
  const char *expr = "sin(-1)*(2+(2*3/4))*cos(5)/tan(0.5)";
  ld result = s21_smart_calc(expr);
  ld expected = sin(-1.0) * (2.0 + (2.0 * 3.0 / 4.0)) * cos(5.0) / tan(0.5);
  ck_assert_double_eq_tol(result, expected, EPSILON);
}
END_TEST

START_TEST(test_ok4) {
  const char *expr =
      "sin(-1.5)*(2+(2*3/"
      "4))*(cos(1)+tan(0.5)+asin(1)+acos(0.5)+atan(0.5)+sqrt(144)+log(1)+ln(1)"
      ")";
  ld result = s21_smart_calc(expr);
  ld expected = sin(-1.5) * (2.0 + (2.0 * 3.0 / 4.0)) *
                (cos(1.0) + tan(0.5) + asin(1.0) + acos(0.5) + atan(0.5) +
                 sqrt(144.0) + log(1.0) + log10(1.0));
  ck_assert_double_eq_tol(result, expected, EPSILON);
}
END_TEST

START_TEST(test_ok5) {
  const char *expr = "3*(3*(4-2*5/3)+2*3/4)*(3*(3*(4-2*5/3)+2*3/4))*cos(5)/3";
  ld result = s21_smart_calc(expr);
  ld expected = 3.0 * (3.0 * (4.0 - 2.0 * 5.0 / 3.0) + 2.0 * 3.0 / 4.0) *
                (3.0 * (3.0 * (4.0 - 2.0 * 5.0 / 3.0) + 2.0 * 3.0 / 4.0)) *
                cos(5.0) / 3.0;
  ck_assert_double_eq_tol(result, expected, EPSILON);
}
END_TEST

START_TEST(test_invalid_expr1) {
  const char *expr = "(2+3))";
  ld result = s21_smart_calc(expr);
  ck_assert_double_eq(result, BRACKETS_NOT_MATCH);
}
END_TEST

START_TEST(test_invalid_expr2) {
  const char *expr = "33++++";
  ld result = s21_smart_calc(expr);
  ck_assert_double_eq(result, INVALID_EXPRESSION);
}
END_TEST

START_TEST(test_invalid_expr3) {
  const char *expr = "privet(33)";
  ld result = s21_smart_calc(expr);
  ck_assert_double_eq(result, UNKNOWN_FUNC);
}
END_TEST

START_TEST(test_invalid_expr4) {
  const char *expr = "";
  ld result = s21_smart_calc(expr);
  ck_assert_double_eq(result, INVALID_EXPRESSION);
}
END_TEST

START_TEST(test_credit_calc_annuint) {
  credit_data result = {0};
  credit_data expected = {8560.7481788, 2728.9781461, 102728.9781461, {0}};

  result = s21_credit_calc(100000, 12, 5, 0);

  ck_assert_double_eq_tol(result.monthly_payment, expected.monthly_payment,
                          EPSILON);
  ck_assert_double_eq_tol(result.overpayment, expected.overpayment, EPSILON);
  ck_assert_double_eq_tol(result.total_payment, expected.total_payment,
                          EPSILON);
}
END_TEST

START_TEST(test_credit_calc_diff) {
  credit_data result = {0};
  ld months[12] = {8750.0,          8715.2777777, 8680.5555555,  8645.83333333,
                   8611.11111111,   8576.3888888, 8541.6666666,  8506.944444444,
                   8472.2222222222, 8437.5,       8402.77777777, 8368.0555555};

  credit_data expected = {0, 2708.3333333, 102708.3333333, {0}};

  for (int i = 0; i < 12; i++) {
    expected.diff_mouths_payment[i] = months[i];
  }

  result = s21_credit_calc(100000, 12, 5, 1);

  ck_assert_double_eq_tol(result.monthly_payment, expected.monthly_payment,
                          EPSILON);
  ck_assert_double_eq_tol(result.overpayment, expected.overpayment, EPSILON);
  ck_assert_double_eq_tol(result.total_payment, expected.total_payment,
                          EPSILON);
  for (int i = 0; i < 12; i++) {
    ck_assert_double_eq_tol(result.diff_mouths_payment[i],
                            expected.diff_mouths_payment[i], EPSILON);
  }
}
END_TEST

START_TEST(test_deposit_calc_no_cap) {
  deposit_data result = {0};
  deposit_data expected = {12230.1369863, 0.0, 112230.1369863};

  result = s21_deposit_calc(100000.0, 12.0, 12.0, 0.0, 30, 10000.0, 10000.0, 0);

  ck_assert_double_eq_tol(result.acc_interest, expected.acc_interest, EPSILON);
  ck_assert_double_eq_tol(result.dep_total, expected.dep_total, EPSILON);
  ck_assert_double_eq_tol(result.tax_total, expected.tax_total, EPSILON);
}
END_TEST

START_TEST(test_deposit_calc_cap) {
  deposit_data result = {0};
  deposit_data expected = {14022.1737565, 0.0, 114022.1737565};

  result = s21_deposit_calc(100000.0, 12.0, 12.0, 0.0, 30, 10000.0, 10000.0, 1);

  ck_assert_double_eq_tol(result.acc_interest, expected.acc_interest, EPSILON);
  ck_assert_double_eq_tol(result.dep_total, expected.dep_total, EPSILON);
  ck_assert_double_eq_tol(result.tax_total, expected.tax_total, EPSILON);
}
END_TEST

Suite *s21_smart_calc_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("S21_Smart_Calc");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_ok1);
  tcase_add_test(tc_core, test_ok2);
  tcase_add_test(tc_core, test_ok3);
  tcase_add_test(tc_core, test_ok4);
  tcase_add_test(tc_core, test_ok5);

  tcase_add_test(tc_core, test_invalid_expr1);
  tcase_add_test(tc_core, test_invalid_expr2);
  tcase_add_test(tc_core, test_invalid_expr3);
  tcase_add_test(tc_core, test_invalid_expr4);

  tcase_add_test(tc_core, test_credit_calc_annuint);
  tcase_add_test(tc_core, test_credit_calc_diff);

  tcase_add_test(tc_core, test_deposit_calc_no_cap);
  tcase_add_test(tc_core, test_deposit_calc_cap);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_smart_calc_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}