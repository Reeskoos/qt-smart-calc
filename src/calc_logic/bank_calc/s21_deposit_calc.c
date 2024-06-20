/**
 * @file
 * @brief Contains functions for calculating deposit data
 */

#include "include/s21_deposit_calc.h"

/**
 * @brief Calculate the capital for a given deposit
 *
 * @param num the initial deposit amount
 * @param rate the annual interest rate
 * @param pay_frequency the number of payment frequencies per year
 * @param term the deposit term in years
 * @return the calculated capital
 */
ld s21_calculate_cap(ld num, ld rate, ld pay_frequency, ld term) {
  return (num * pow(1 + (rate / pay_frequency / 1200), pay_frequency * term)) -
         num;
}

/**
 * @brief Calculate the deposit data including interest and tax
 *
 * @param amount the initial deposit amount
 * @param term the deposit term in years
 * @param rate the annual interest rate
 * @param tax the tax rate
 * @param pay_frequency the number of payment frequencies per year
 * @param replen the replenishment amount
 * @param withdrawals the withdrawal amount
 * @param capital_percent the percentage of capital to consider
 * @return the calculated deposit data
 */
deposit_data s21_deposit_calc(ld amount, ld term, ld rate, ld tax,
                              ld pay_frequency, ld replen, ld withdrawals,
                              int capital_percent) {
  deposit_data res = {0};
  if ((amount <= 0 || term <= 0 || rate <= 0 || pay_frequency <= 0)) {
    res.dep_total = DEPOSIT_ERROR;
    return res;
  }

  res.dep_total = amount;
  if (!capital_percent) {
    res.dep_total += replen - withdrawals;
    res.acc_interest = res.dep_total * rate * (term * 31 / 365) / 100;
    res.tax_total = res.dep_total * tax / 100;
    res.dep_total += res.acc_interest - res.tax_total;
  } else {
    ld total_cap = s21_calculate_cap(res.dep_total, rate, pay_frequency, term);
    ld replen_cap = s21_calculate_cap(replen, rate, pay_frequency, term);

    res.acc_interest = total_cap + replen_cap;
    res.tax_total = res.acc_interest * tax / 100;
    res.dep_total += replen + res.acc_interest - res.tax_total - withdrawals;
  }

  return res;
}