/**
 * @file
 * @brief Contains functions for expression validation
 */

#include "include/translator.h"

/**
 * @brief Checks if the character is an operator
 *
 * @param expr The expression to check
 * @param iter The iterator pointing to the current position in the expression
 * @return 1 if the character is an operator, 0 otherwise
 */
int s21_is_oper(const char *expr, int *iter) {
  int res = 0;

  if ((expr[*iter] >= 42 && expr[*iter] <= 47) || expr[*iter] == '^') {
    res = 1;
    (*iter)++;
  }

  if (expr[*iter] == 44 || expr[*iter] == 46) {
    res = 0;
    (*iter)++;
  } else if (expr[*iter - 1] == '(' && *expr == '-') {
    res = 0;
    (*iter)++;
  } else if (expr[*iter - 1] == '(' && *expr == '+') {
    res = 0;
    (*iter)++;
  }

  return res;
}

/**
 * @brief Checks if the character is a number
 *
 * @param expr The expression to check
 * @param iter The iterator pointing to the current position in the expression
 * @return 1 if the character is a number, 0 otherwise
 */
int s21_is_number(const char *expr, int *iter) {
  int res = 0;

  if (isdigit(expr[*iter])) {
    if (expr[*iter] == '0' && isdigit(expr[*iter + 1])) {
      res = INVALID_EXPRESSION;
    } else {
      res = 1;

      while (isdigit(expr[*iter])) {
        (*iter)++;
        if (expr[*iter] == '\0') break;
        if (expr[*iter] == '.') (*iter)++;
      }
    }
  }
  return res;
}

/**
 * @brief Checks if the function is correct
 *
 * @param expr The expression to check
 * @param iter The iterator pointing to the current position in the expression
 * @return 1 if the function is correct, 0 otherwise
 */
int s21_is_correct_func(const char *expr, int *iter) {
  int res = 1;
  char func[255] = {0};

  if (expr) {
    s21_read_funcs(expr, iter, func);
    if (func[0] != '\0') {
      oper_data cur_func = s21_init_functions(func);
      if (cur_func.type == NO_TYPE) res = UNKNOWN_FUNC;
    } else {
      res = 0;
    }
  }

  return res;
}

/**
 * @brief Checks if the character is a bracket
 *
 * @param expr The expression to check
 * @param iter The iterator pointing to the current position in the expression
 * @param data The validation data struct to store the number of brackets
 * @return 1 if the character is a bracket, 0 otherwise
 */
int s21_is_bracket(const char *expr, int *iter, validation_data *data) {
  int res = 0;

  if (expr[*iter] == ')') {
    data->right_brackets++;
    res = 1;
    (*iter)++;
  } else if (expr[*iter] == '(') {
    (*iter)++;
    data->left_brackets++;
    res = 1;

    if (isdigit(expr[*iter])) {
      int tmp_iter = *iter;
      s21_is_number(expr, &tmp_iter);
      if (expr[tmp_iter] == ')' && !isalpha(expr[*iter - 2])) {
        res = INVALID_EXPRESSION;
      }
    } else if (s21_is_oper(expr, iter)) {
      if (expr[*iter] == '(') res = INVALID_EXPRESSION;
    }
  }

  return res;
}

/**
 * @brief Collects data about the expression for validation
 *
 * @param expr The expression to collect data from
 * @return The validation data struct containing the number of brackets,
 * functions, operators, and numbers
 */
validation_data s21_collect_data(const char *expr) {
  validation_data res = {0};
  int iter = 0;
  int stop = 0;
  int func_code = 0;
  int is_num = 0;

  while (expr[iter] != '\0' && !stop) {
    if (s21_is_bracket(expr, &iter, &res) == INVALID_EXPRESSION) {
      res.left_brackets = INVALID_EXPRESSION;
      stop = 1;
    } else {
      func_code = s21_is_correct_func(expr, &iter);
      if (func_code == 1) {
        res.funcs++;
      } else if (func_code == UNKNOWN_FUNC) {
        res.funcs = UNKNOWN_FUNC;
        stop = 1;
      } else {
        if (s21_is_oper(expr, &iter)) {
          if (s21_is_oper(expr, &iter)) {
            res.opers = INVALID_EXPRESSION;
            stop = 1;
          } else {
            res.opers++;
          }
        }
        is_num = s21_is_number(expr, &iter);
        if (is_num == INVALID_EXPRESSION) {
          res.nums = INVALID_EXPRESSION;
          stop = 1;
        } else if (is_num == 1) {
          res.nums++;
        }
      }
    }
  }

  return res;
}

/**
 * @brief Validates the expression
 *
 * @param expr The expression to validate
 * @return The validation result code
 */
int s21_expr_validation(const char *expr) {
  int res = VALID_OK;
  validation_data data = s21_collect_data(expr);

  if (data.opers == INVALID_EXPRESSION) {
    res = INVALID_EXPRESSION;
  } else if (data.left_brackets == INVALID_EXPRESSION) {
    res = INVALID_EXPRESSION;
  } else if (data.left_brackets != data.right_brackets) {
    res = BRACKETS_NOT_MATCH;
  } else if (data.opers >= data.nums && data.funcs == 0) {
    res = INVALID_EXPRESSION;
  } else if (data.opers == 0 && data.nums == 0 && data.funcs == 0) {
    res = INVALID_EXPRESSION;
  } else if (data.funcs == UNKNOWN_FUNC) {
    res = UNKNOWN_FUNC;
  } else if (data.funcs != data.nums && data.opers == 0) {
    res = INVALID_EXPRESSION;
  } else if (data.nums == INVALID_EXPRESSION) {
    res = INVALID_EXPRESSION;
  }

  return res;
}