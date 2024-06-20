#ifndef S21_STACK_H
#define S21_STACK_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum stack_return_codes { STACK_OK, STACK_ERROR };

typedef struct Stack {
  int count;
  size_t size;
  long double *value;
} Stack;

Stack *s21_create_stack(size_t size);
int s21_push(Stack *stack, long double value);
long double s21_pop(Stack *stack);
long double s21_top(Stack const *stack);
void s21_clear_stack(Stack *stack);
int s21_is_stack_empty(const Stack *stack);
int s21_is_stack_full(const Stack *stack);

#endif  // S21_STACK_H