#ifndef S21_DEPOSIT_CALC_H
#define S21_DEPOSIT_CALC_H

#define DEPOSIT_ERROR -336
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long double ld;

typedef struct deposit_output_data {
  ld acc_interest;
  ld tax_total;
  ld dep_total;
} deposit_data;

deposit_data s21_deposit_calc(ld amount, ld term, ld rate, ld tax,
                              ld pay_frequency, ld replen, ld withdrawals,
                              int capital_percent);

#ifdef __cplusplus
}
#endif

#endif  // S21_DEPOSIT_CALC_H