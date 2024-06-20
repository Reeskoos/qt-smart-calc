#ifndef S21_CALC_H
#define S21_CALC_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/include/s21_operators_stack.h"
#include "stack/include/s21_stack.h"

#ifdef __cplusplus
extern "C" {
#endif

long double s21_smart_calc(const char *expr);
void s21_exec_calc(Stack *nums, OperStack *oper_stack);

#ifdef __cplusplus
}
#endif

#endif  // S21_CALC_H
