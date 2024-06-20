#ifndef S21_OPERATORS_STACK_H
#define S21_OPERATORS_STACK_H

#include "s21_stack.h"

enum oper_type { NO_TYPE, LEFT_BRACKET, RIGHT_BRACKET, OPERAND, FUNC };

typedef struct oper_data {
  char value;
  enum oper_type type;
  int priority;
  double (*math_func)(double);
} oper_data;

typedef struct OperStack {
  int count;
  size_t size;
  struct oper_data *data;
} OperStack;

OperStack *s21_create_oper_stack(size_t size);
int s21_push_oper(OperStack *stack, oper_data data);
oper_data s21_pop_oper(OperStack *stack);
oper_data s21_top_oper(const OperStack *stack);
void s21_clear_oper_stack(OperStack *stack);
int s21_is_oper_stack_empty(const OperStack *stack);
int s21_is_oper_stack_full(const OperStack *stack);

#endif  // S21_OPERATORS_STACK_H