/**
 * @file
 * @brief Contains utils functions for translator module
 */

#include "include/translator.h"

/**
 * @brief Reads and stores a function from the expression into the buffer.
 *
 * @param expression The input expression.
 * @param exp_iter Pointer to the current position in the expression.
 * @param buffer The buffer to store the function.
 */
void s21_read_funcs(const char *expression, int *exp_iter, char *buffer) {
  if (expression && buffer) {
    if (expression[*exp_iter] >= 'a' && expression[*exp_iter] <= 'z') {
      while (expression[*exp_iter] >= 'a' && expression[*exp_iter] <= 'z') {
        strncat(buffer, &expression[*exp_iter], 1);
        (*exp_iter)++;
      }
    }
  }
}

/**
 * @brief Extracts a number from the string expression at the given position.
 *
 * @param expr The input expression.
 * @param iter Pointer to the current position in the expression.
 * @return The extracted number.
 */
long s21_num_from_str(const char *expr, int *iter) {
  char *endptr = NULL;
  long buffer = strtol((char *)(expr + *iter), &endptr, 10);
  *iter += endptr - (expr + *iter);
  return buffer;
}

/**
 * @brief Reads a fraction from the expression and calculates its value.
 *
 * @param expr The input expression.
 * @param exp_iter Pointer to the current position in the expression.
 * @param int_part The integer part of the number.
 * @param unary_minus Flag indicating whether the fraction is unary minus.
 * @return The value of the fraction.
 */
long double s21_read_fraction(const char *expr, int *exp_iter, int int_part,
                              int unary_minus) {
  int tmp = 0;
  long double res = 0.0;

  if (isdigit(expr[*exp_iter])) {
    int save_iter = *exp_iter;
    tmp = s21_num_from_str(expr, exp_iter);
    if (unary_minus) {
      res = int_part - (tmp / pow(10, (*exp_iter - save_iter)));
    } else {
      res = int_part + (tmp / pow(10, (*exp_iter - save_iter)));
    }
  } else {
    res = NO_NUM;
  }
  return res;
}
/**
 * @brief Checks for a unary operator in the expression.
 *
 * @param expr The input expression.
 * @param exp_iter The current position in the expression.
 * @param cur_oper The current operator being checked.
 * @param unary Pointer to store the unary flag.
 * @return 1 if a unary operator is found, 0 otherwise.
 */
int s21_check_unary_oper(const char *expr, int exp_iter, oper_data cur_oper,
                         int *unary) {
  int res = 0;
  if (expr[exp_iter - 2] == '(' && cur_oper.value == '-') {
    *unary = 1;
    res = 1;
  } else if (expr[exp_iter - 2] == '(' && cur_oper.value == '+') {
    *unary = 0;
    res = 1;
  }
  return res;
}

/**
 * @brief Checks if the current operator's type is not 'f' or 'NO_TYPE' and not
 * a closing bracket. Made to return iterator to last oper position.
 *
 * @param cur_oper The current operator being checked.
 * @return 1 if the conditions are met, 0 otherwise.
 */
int s21_check_iter_offset(oper_data cur_oper) {
  int res = 0;
  if (cur_oper.type != 'f' && cur_oper.type != NO_TYPE &&
      cur_oper.value != ')') {
    res = 1;
  }
  return res;
}
/**
 * @brief Checks the priority of the current operator against the previous one
 * in the stack.
 *
 * @param oper_stack The stack of operators.
 * @param cur_oper The current operator being checked.
 * @return 1 if the priority conditions are met, 0 otherwise.
 */
int s21_check_oper_priority(OperStack *oper_stack, oper_data cur_oper) {
  int res = 0;
  oper_data prev_oper = s21_top_oper(oper_stack);

  if ((cur_oper.priority < prev_oper.priority &&
       cur_oper.type != LEFT_BRACKET && prev_oper.type != NO_TYPE)) {
    res = 1;
  } else if (prev_oper.priority == cur_oper.priority || cur_oper.value == ')') {
    res = 1;
  }
  return res;
}
