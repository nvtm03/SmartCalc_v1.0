#include "s21_smart_calc_test.h"

START_TEST(s21_test_0) {
  double x = 0;
  double orig = 10;
  double result = 0.0;
  // int error = 0;
  char str[255] = "5 + 5";
  x = s21_smart_calc(str, 0.0, &result);
  ck_assert_int_eq(0, x);
  ck_assert_double_eq(result, orig);
}
END_TEST

START_TEST(s21_test_1) {
  double x = 0;
  double orig = 162;
  // int error = 0;
  double result = 0.0;
  char str[255] = "   4 * 5 * 2 * 2 * 2 + 2";
  x = s21_smart_calc(str, 0.0, &result);
  ck_assert_int_eq(0, x);
  ck_assert_double_eq(result, orig);
}
END_TEST

START_TEST(s21_test_2) {
  double x = 0;
  double orig = 20;
  // int error = 0;
  double result = 0.0;
  char str[255] = "  150 / 10 + 5 * 2 / 2";
  x = s21_smart_calc(str, 0.0, &result);
  ck_assert_int_eq(0, x);
  ck_assert_double_eq(result, orig);
}
END_TEST

START_TEST(s21_test_3) {
  double x = 0;
  double orig = 9;
  // int error = 0;
  double result = 0.0;
  char str[255] = "  5 + 5 - 2 * 3 / 6";
  x = s21_smart_calc(str, 0.0, &result);
  ck_assert_int_eq(0, x);
  ck_assert_double_eq(result, orig);
}
END_TEST

START_TEST(calc_test) {
  {
    char src[100] = "4 ^ acos( x / 4) / tan( 2 * x)";
    double x_text = 1.2;
    double result = 0.0;
    int res = s21_smart_calc(src, x_text, &result);
    double real_result = -6.31492;
    ck_assert_int_eq(0, res);
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-1 failed");
  }
  {
    char src[100] = "x+x";
    double x_text = 2;
    double result = 0.0;
    int res = s21_smart_calc(src, x_text, &result);
    double real_result = 4;
    ck_assert_int_eq(0, res);
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-2 failed");
  }
  {
    char src[100] = "123 + 0.456";
    double result = 0.0;
    int res = s21_smart_calc(src, 0.0, &result);
    ck_assert_int_eq(0, res);
    double real_result = 123.456;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-3 failed");
  }
  {
    char src[100] = "log(x)";
    double x_text = -2;
    double real_result = NAN;
    double result = 0.0;
    int res = s21_smart_calc(src, x_text, &result);
    ck_assert_int_eq(0, res);
    ck_assert_msg(result != result && real_result != real_result,
                  "test-4 failed");
  }
  {
    char src[100] = " - ( - 1)";
    double result = 0.0;
    int res = s21_smart_calc(src, 0.0, &result);
    ck_assert_int_eq(0, res);
    double real_result = 1;
    ck_assert_msg(result == real_result, "test-5 failed");
  }
  {
    char src[100] = "cos(10 mod 2)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(0, res);
    double real_result = 1;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-6 failed");
  }
  {
    char src[100] = "sqrt(ln(10))";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(0, res);
    double real_result = 1.517427;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-7 failed");
  }
  {
    char src[100] = "atan(10) + sin(10)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(0, res);
    double real_result = 1.644775;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-8 failed");
  }
  {
    char src[100] = "asin(1)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(0, res);
    double real_result = 1.570796;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001, "test-9 failed");
  }
  {
    char src[100] = "10 - 20 * ( - 10)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(0, res);
    double real_result = -210;
    ck_assert_msg(fabs(result) - fabs(real_result) <= 0.00001,
                  "test-10 failed");
  }
  {
    char src[100] = " - (o(i(a(10.01) * n(2)) / 10 m 2)) ^ q(5)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
  {
    char src[100] = " - (o(i(a(10.01) * n(2)) / 10m(2)) ^ q(5)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
  {
    char src[100] = "mod + 9(()))";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
  {
    char src[100] = "md( + 9(()))";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
  {
    char src[100] = " - (o(i(a(10.01) * n(2)) / 10 md + (2)) ^ q(5)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
  {
    char src[100] = ". + m)";
    float x_test = 0.0;
    double result = 0.0;
    int res = s21_smart_calc(src, x_test, &result);
    ck_assert_int_eq(ERROR, res);
  }
}
END_TEST

START_TEST(s21_test_4) {
  double orig = 69;
  // int error = 0;

  char str[255] = " 2 + (5 * 5 * 5 + 5 + 2 * 2) / (2)";

  float x_test = 0.0;
  double result = 0.0;
  int res = s21_smart_calc(str, x_test, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(result, orig);
}
END_TEST

END_TEST
Suite *suite_smart_calc(void) {
  Suite *s = suite_create("suite_smart_calc");
  TCase *tc = tcase_create("suite_smart_calc");

  tcase_add_test(tc, s21_test_0);
  tcase_add_test(tc, s21_test_1);
  tcase_add_test(tc, s21_test_2);
  tcase_add_test(tc, s21_test_3);

  tcase_add_test(tc, calc_test);
  tcase_add_test(tc, s21_test_4);
  suite_add_tcase(s, tc);
  return s;
}
