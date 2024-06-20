/**
 * @file
 * @brief Contains functions for calculating credit data
 */

#include "include/s21_credit_calc.h"

/**
 * @brief Calculate the annuity payment, total payment, and overpayment for a
 * credit.
 *
 * @param amount The amount of the credit.
 * @param rate The annual interest rate.
 * @param term The term of the credit in months.
 * @return The calculated credit data including monthly payment, total payment,
 * and overpayment.
 */
credit_data s21_calc_annuity(ld amount, ld rate, ld term) {
  credit_data res = {0};
  ld monthly_rate = rate / 100.0 / 12.0;

  res.monthly_payment = amount * (monthly_rate * pow(1 + monthly_rate, term)) /
                        (pow(1 + monthly_rate, term) - 1);
  res.total_payment = res.monthly_payment * term;
  res.overpayment = res.total_payment - amount;

  return res;
}

/**
 * @brief Calculate the differentiated monthly payment for a specific month of a
 * credit.
 *
 * @param amount The amount of the credit.
 * @param rate The annual interest rate.
 * @param term The term of the credit in months.
 * @param month The specific month for which to calculate the payment.
 * @return The calculated differentiated monthly payment.
 */
ld s21_calc_diff_month(ld amount, ld rate, ld term, int month) {
  ld monthly_rate = rate / 12 / 100;
  ld monthly_payment =
      (amount / term) + (amount - (month - 1) * (amount / term)) * monthly_rate;
  return monthly_payment;
}

/**
 * @brief Calculate the credit data including annuity or differentiated payment,
 * total payment, and overpayment.
 *
 * @param amount The amount of the credit.
 * @param term The term of the credit in months.
 * @param rate The annual interest rate.
 * @param type The type of payment calculation (0 for annuity, 1 for
 * differentiated).
 * @return The calculated credit data including payment details.
 */
credit_data s21_credit_calc(ld amount, ld term, ld rate, int type) {
  credit_data res = {0};
  if ((amount <= 0 || term <= 0 || rate <= 0) || term >= 120) {
    res.total_payment = CREDIT_ERROR;
    return res;
  }

  if (!type) {  // annuity
    res = s21_calc_annuity(amount, floorl(rate), term);
  } else {  // differentiated
    for (int i = 1; i <= term; i++) {
      res.diff_mouths_payment[i - 1] =
          s21_calc_diff_month(amount, floorl(rate), term, i);
      res.total_payment += res.diff_mouths_payment[i - 1];
      res.overpayment += res.diff_mouths_payment[i - 1] - amount / term;
    }
  }
  return res;
}
