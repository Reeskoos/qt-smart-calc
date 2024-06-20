#ifndef S21_CREDIT_CALC_H
#define S21_CREDIT_CALC_H

#define CREDIT_ERROR -335
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long double ld;

typedef struct credit_output_data {
  ld monthly_payment;
  ld overpayment;
  ld total_payment;
  ld diff_mouths_payment[120];

} credit_data;

credit_data s21_credit_calc(ld amount, ld term, ld rate, int type);

#ifdef __cplusplus
}
#endif

#endif  // S21_CREDIT_CALC_H