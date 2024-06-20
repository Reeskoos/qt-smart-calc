/**
 * @file
 * @brief Contains functions for expression translation into stacks
 */

#include "include/translator.h"

/**
 * @brief Initializes an operand based on the given character
 *
 * @param c The character representing the operand
 * @return oper_data The initialized operand data
 */
oper_data s21_init_oper(char c) {
  oper_data res = {0};
  switch (c) {
    case '+':
      res.value = '+';
      res.type = OPERAND;
      res.priority = 2;
      break;
    case '-':
      res.value = '-';
      res.type = OPERAND;
      res.priority = 2;
      break;
    case '*':
      res.value = '*';
      res.type = OPERAND;
      res.priority = 3;
      break;
    case ':':
    case '/':
      res.value = '/';
      res.type = OPERAND;
      res.priority = 3;
      break;
    case '(':
      res.value = '(';
      res.type = LEFT_BRACKET;
      res.priority = 1;
      break;
    case ')':
      res.value = ')';
      res.type = RIGHT_BRACKET;
      res.priority = 1;
      break;
    case '^':
      res.value = '^';
      res.type = OPERAND;
      res.priority = 5;
      break;
    default:
      res.type = 0;
      res.value = 0;
      res.priority = 0;
      break;
  }
  res.math_func = NULL;
  return res;
}

/**
 * @brief Initializes a function based on the given function name
 *
 * @param func The name of the function
 * @return oper_data The initialized function data
 */
oper_data s21_init_functions(char func[10]) {
  oper_data res = {0};

  res.type = FUNC;
  res.priority = 10;
  res.value = 'f';

  if (!strcmp(func, "cos")) {
    res.math_func = cos;
  } else if (!strcmp(func, "sin")) {
    res.math_func = sin;
  } else if (!strcmp(func, "tan")) {
    res.math_func = tan;
  } else if (!strcmp(func, "acos")) {
    res.math_func = acos;
  } else if (!strcmp(func, "asin")) {
    res.math_func = asin;
  } else if (!strcmp(func, "atan")) {
    res.math_func = atan;
  } else if (!strcmp(func, "sqrt")) {
    res.math_func = sqrt;
  } else if (!strcmp(func, "ln")) {
    res.math_func = log10;
  } else if (!strcmp(func, "log")) {
    res.math_func = log;
  } else {
    res.type = NO_TYPE;
  }

  return res;
}

/**
 * @brief Reads the next operand or function from the expression
 *
 * @param opers The operand stack
 * @param expression The expression string
 * @param exp_iter The expression iterator
 * @return oper_data The read operand or function data
 */
oper_data s21_read_oper(OperStack *opers, const char *expression,
                        int *exp_iter) {
  oper_data res = {0};
  if (expression) {
    char func[255] = {0};
    s21_read_funcs(expression, exp_iter, func);
    if (func[0] == '\0') {
      switch (expression[*exp_iter]) {
        case '+':
          res = s21_init_oper('+');
          break;
        case '-':
          res = s21_init_oper('-');
          break;
        case '*':
          res = s21_init_oper('*');
          break;
        case '/':
          res = s21_init_oper('/');
          break;
        case '(':
          res = s21_init_oper('(');
          break;
        case ')':
          res = s21_init_oper(')');
          break;
        case '^':
          res = s21_init_oper('^');
          break;
        default:
          res = s21_init_oper('z');
      }
    } else {
      res = s21_init_functions(func);
    }
    memset(func, 0, 255);
    if (opers && res.value && res.value != 'f') {
      (*exp_iter)++;
    }
  }

  return res;
}
/**
 * @brief Reads the entire expression and sets up the operand and number stacks,
 *       returns if calculation is necessary
 *
 *
 * @param expr The expression string
 * @param nums The number stack
 * @param oper_stack The operand stack
 * @param iter The iterator for the expression
 * @return int The updated expression iterator
 */
int s21_read_expression(const char *expr, Stack *nums, OperStack *oper_stack,
                        int iter) {
  int exp_iter = iter;
  int unary = 0;
  int is_number = 0;
  int calc_necessary = 0;

  while (expr[exp_iter] != '\0' && !calc_necessary) {
    if (expr[exp_iter] == ' ') {
      exp_iter++;
      continue;
    }

    int tmp = 0;
    long double into_stack = 0;

    if (isdigit(expr[exp_iter])) {
      tmp = s21_num_from_str(expr, &exp_iter);
      if (unary) tmp = -tmp;
      is_number = 1;
    }

    if (expr[exp_iter] == '.') {
      exp_iter++;
      into_stack = s21_read_fraction(expr, &exp_iter, tmp, unary);
      if (into_stack != NO_NUM) s21_push(nums, into_stack);
    } else if (is_number) {
      s21_push(nums, (long double)tmp);
    }

    oper_data cur_oper = s21_read_oper(oper_stack, expr, &exp_iter);
    if (!s21_check_unary_oper(expr, exp_iter, cur_oper, &unary)) {
      if (s21_check_oper_priority(oper_stack, cur_oper)) {
        calc_necessary = 1;
        if (s21_check_iter_offset(cur_oper)) exp_iter--;
      }

      if (cur_oper.type != NO_TYPE) {
        if (!calc_necessary || cur_oper.type == RIGHT_BRACKET) {
          s21_push_oper(oper_stack, cur_oper);
        }
      }

      unary = 0;
    }
    is_number = 0;
  }
  return exp_iter;
}
