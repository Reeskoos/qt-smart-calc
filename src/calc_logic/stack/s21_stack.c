/**
 * @file
 * @brief Contains functions for numbers stack
 */

#include "include/s21_stack.h"

/**
 * @brief Creates a new stack with the specified size.
 *
 * @param size The size of the stack to be created.
 * @return A pointer to the newly created stack, or NULL if memory allocation
 * fails.
 */
Stack *s21_create_stack(size_t size) {
  Stack *res = calloc(1, sizeof(Stack));
  if (res) {
    res->count = -1;
    res->value = NULL;
    res->size = 0;

    if (size) {
      res->value = calloc(size, sizeof(double));
      if (res->value != NULL) {
        res->size = size;
      }
    }
  }
  return res;
}

/**
 * @brief Pushes a value onto the stack.
 *
 * @param stack The stack to push the value onto.
 * @param value The value to be pushed onto the stack.
 * @return STACK_OK if successful, or STACK_ERROR if the stack is full or NULL.
 */
int s21_push(Stack *stack, long double value) {
  if (!stack) return STACK_ERROR;

  int err = STACK_OK;

  if (!s21_is_stack_full(stack)) {
    ++(stack->count);
    stack->value[stack->count] = value;
  } else {
    err = STACK_ERROR;
  }

  return err;
}

/**
 * @brief Pops a value from the stack.
 *
 * @param stack The stack to pop the value from.
 * @return The value popped from the stack, or 0 if the stack is empty or NULL.
 */
long double s21_pop(Stack *stack) {
  long double res = 0;
  if (stack) {
    if (!s21_is_stack_empty(stack)) {
      res = stack->value[stack->count];
      stack->value[stack->count] = 0.0;
      stack->count--;
    }
  }
  return res;
}

/**
 * @brief Returns the top value of the stack without removing it.
 *
 * @param stack The stack to retrieve the top value from.
 * @return The top value of the stack, or 0 if the stack is empty or NULL.
 */
long double s21_top(Stack const *stack) {
  long double res = 0;
  if (stack) {
    if (!s21_is_stack_empty(stack)) {
      res = stack->value[stack->count];
    }
  }
  return res;
}

/**
 * @brief Clears the stack and frees memory.
 *
 * @param stack The stack to be cleared and freed.
 */
void s21_clear_stack(Stack *stack) {
  if (stack) {
    if (stack->value) {
      free(stack->value);
      stack->count = 0;
      stack->size = 0;
    }
    free(stack);
  }
}

/**
 * @brief Checks if the stack is empty.
 *
 * @param stack The stack to be checked.
 * @return STACK_OK if the stack is empty, or STACK_ERROR if the stack is NULL.
 */
int s21_is_stack_empty(const Stack *stack) {
  if (!stack) return STACK_ERROR;

  return stack->count < 0;
}

/**
 * @brief Checks if the stack is full.
 *
 * @param stack The stack to be checked.
 * @return STACK_OK if the stack is full, or STACK_ERROR if the stack is NULL.
 */
int s21_is_stack_full(const Stack *stack) {
  if (!stack) return STACK_ERROR;

  return stack->count == (int)stack->size - 1;
}
