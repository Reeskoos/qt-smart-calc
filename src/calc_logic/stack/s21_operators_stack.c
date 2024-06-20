/**
 * @file
 * @brief Contains functions for operations stack
 */

#include "include/s21_operators_stack.h"

/**
 * @brief Create a new operation stack
 *
 * @param size The size of the stack
 * @return OperStack* A pointer to the newly created stack
 */
OperStack *s21_create_oper_stack(size_t size) {
  OperStack *res = calloc(1, sizeof(OperStack));
  if (res) {
    res->count = -1;
    res->data = NULL;
    res->size = 0;

    if (size) {
      res->data = calloc(size, sizeof(oper_data));
      if (res->data != NULL) {
        res->size = size;
      }
    }
  }
  return res;
}

/**
 * @brief Pushes an element onto the operation stack.
 *
 * @param stack The operation stack to push the element onto.
 * @param data The data to push onto the stack.
 * @return int Returns STACK_ERROR if the stack is null, otherwise returns
 * STACK_OK.
 */
int s21_push_oper(OperStack *stack, oper_data data) {
  if (!stack) return STACK_ERROR;

  int err = STACK_OK;
  if (!s21_is_oper_stack_full(stack)) {
    ++(stack->count);
    stack->data[stack->count] = data;
  } else {
    err = STACK_ERROR;
  }
  return err;
}
/**
 * @brief Pops an element from the operation stack.
 *
 * @param stack The operation stack to pop the element from.
 * @return oper_data Returns the popped data.
 */
oper_data s21_pop_oper(OperStack *stack) {
  oper_data res = {0};
  if (stack) {
    if (!s21_is_oper_stack_empty(stack)) {
      res = stack->data[stack->count];
      stack->count--;
    }
  }
  return res;
}

/**
 * @brief Returns the top element of the operation stack.
 *
 * @param stack The operation stack to get the top element from.
 * @return oper_data Returns the top data.
 */
oper_data s21_top_oper(const OperStack *stack) {
  oper_data res = {0};

  if (!s21_is_oper_stack_empty(stack)) {
    res = stack->data[stack->count];
  }

  return res;
}

/**
 * @brief Clears the operation stack.
 *
 * @param stack The operation stack to clear.
 */
void s21_clear_oper_stack(OperStack *stack) {
  if (stack) {
    if (stack->data) {
      free(stack->data);
      stack->count = 0;
      stack->size = 0;
    }
    free(stack);
  }
}
/**
 * @brief Clears the operation stack.
 *
 * @param stack The operation stack to clear.
 */
int s21_is_oper_stack_empty(const OperStack *stack) {
  if (!stack) return STACK_ERROR;
  return stack->count == -1;
}
/**
 * @brief Checks if the operation stack is full.
 *
 * @param stack The operation stack to check.
 * @return int Returns STACK_ERROR if the stack is null, otherwise returns 1 if
 * the stack is full, 0 otherwise.
 */
int s21_is_oper_stack_full(const OperStack *stack) {
  if (!stack) return STACK_ERROR;
  return stack->count == (int)stack->size - 1;
}