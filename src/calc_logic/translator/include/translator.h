#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../../stack/include/s21_operators_stack.h"
#include "../../stack/include/s21_stack.h"

#define NO_NUM -335

/* VALIDATION */
enum validation_error_codes {
  VALID_OK,
  BRACKETS_NOT_MATCH = 1569325041,
  INVALID_EXPRESSION = 1569325042,
  UNKNOWN_FUNC = 1569325043,
  STR_OVERFLOW = 1569325044,
  NULL_PTR = 1569325045,
};
typedef struct validation_data {
  int left_brackets;
  int right_brackets;
  int opers;
  int nums;
  int funcs;
} validation_data;

int s21_is_oper(const char *expr, int *iter);
int s21_is_number(const char *expr, int *iter);
int s21_is_correct_func(const char *expr, int *iter);
validation_data s21_collect_data(const char *expr);
int s21_expr_validation(const char *expr);

/* ================================================= */
/* TRANSLATOR */
oper_data s21_init_oper(char c);
oper_data s21_init_functions(char func[10]);
oper_data s21_read_oper(OperStack *opers, const char *expression,
                        int *exp_iter);
int s21_read_expression(const char *expr, Stack *nums, OperStack *oper_stack,
                        int iter);

void s21_read_funcs(const char *expression, int *exp_iter, char *buffer);
long double s21_read_fraction(const char *expr, int *exp_iter, int int_part,
                              int unary_minus);
long s21_num_from_str(const char *format, int *iter);
int s21_check_unary_oper(const char *expr, int exp_iter, oper_data cur_oper,
                         int *unary);
int s21_check_oper_priority(OperStack *oper_stack, oper_data cur_oper);
int s21_check_iter_offset(oper_data cur_oper);
#endif  // TRANSLATOR_H
