/**
 * @file
 * @brief Contains main calc function
 */

#include "s21_calc.h"

#include "translator/include/translator.h"

/**
 * @brief Perform a calculation based on the operator and operands
 *
 * @param nums Pointer to the stack containing numbers
 * @param oper_stack Pointer to the stack containing operators
 */
void s21_exec_calc(Stack *nums, OperStack *oper_stack) {
  long double tmp = 0;
  oper_data cur_oper = s21_pop_oper(oper_stack);

  if (cur_oper.type != FUNC) {
    long double cur_a = s21_pop(nums);
    long double cur_b = s21_pop(nums);
    if (!isnan(cur_a) && !isnan(cur_b)) {
      if (cur_oper.value == '+') {
        tmp = cur_b + cur_a;
      } else if (cur_oper.value == '-') {
        tmp = cur_b - cur_a;
      } else if (cur_oper.value == '*') {
        tmp = cur_b * cur_a;
      } else if (cur_oper.value == '/') {
        if (cur_a) {
          tmp = cur_b / cur_a;
        } else {
          tmp = NAN;
        }
      } else if (cur_oper.value == '^') {
        tmp = pow(cur_b, cur_a);
      }
    } else {
      tmp = NAN;
    }
  } else {
    long double num = s21_pop(nums);
    if (num < 0 && cur_oper.math_func == sqrt) {
      tmp = NAN;
    } else {
      tmp = cur_oper.math_func(num);
    }
  }
  s21_push(nums, tmp);
}

/**
 * @brief Calculate the result of the given expression.
 *
 * This function calculates the result of the given expression and returns it.
 *
 * @param expr The expression to be evaluated.
 * @return The result of the expression calculation.
 */
long double s21_smart_calc(const char *expr) {
  if (!expr) return NULL_PTR;
  if (strlen(expr) > 255) return STR_OVERFLOW;

  long double res = 0.0;
  res = s21_expr_validation(expr);

  if ((int)res == VALID_OK) {
    int exp_iter = 0;
    Stack *nums = s21_create_stack(150);
    OperStack *oper_stack = s21_create_oper_stack(75);
    int expr_len = strlen(expr);

    while (exp_iter != expr_len) {
      exp_iter = s21_read_expression(expr, nums, oper_stack, exp_iter);
      if (exp_iter && !s21_is_oper_stack_empty(oper_stack)) {
        if (s21_top_oper(oper_stack).type == RIGHT_BRACKET) {
          s21_pop_oper(oper_stack);
          while (s21_top_oper(oper_stack).type != LEFT_BRACKET) {
            s21_exec_calc(nums, oper_stack);
          }
          s21_pop_oper(oper_stack);
        } else {
          s21_exec_calc(nums, oper_stack);
        }
      }
    }

    while (!s21_is_oper_stack_empty(oper_stack)) {
      if (s21_top_oper(oper_stack).type == LEFT_BRACKET) {
        s21_pop_oper(oper_stack);
      } else {
        s21_exec_calc(nums, oper_stack);
      }
    }

    res = s21_pop(nums);
    s21_clear_oper_stack(oper_stack);
    s21_clear_stack(nums);
  }

  return res;
}
