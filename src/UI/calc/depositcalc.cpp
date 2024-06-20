/**
 * @file depositcalc.cpp
 * @brief Contains the implementation of the DepositCalc class for deposit
 * calculation.
 */

#include "depositcalc.h"

#include "ui_depositcalc.h"

/**
 * Constructor for the DepositCalc class.
 *
 * @param parent the parent widget
 *
 */
DepositCalc::DepositCalc(QWidget *parent)
    : QWidget(parent), ui(new Ui::DepositCalc) {
  ui->setupUi(this);
  connect(ui->execButton, SIGNAL(released()), this, SLOT(ExecPressed()));
}

DepositCalc::~DepositCalc() { delete ui; }

/**
 * Validates the parameter by converting it to a double and setting the result
 * to the given reference.
 *
 * @param param the parameter to be validated
 *
 * @return the result of the validation
 *
 * @throws None
 */
bool DepositCalc::ValidateParam(QString param) {
  bool res = false;
  param.toDouble(&res);
  return res;
}

/**
 * FieldsValidation checks if the parameters are valid.
 *
 * @return true if all parameters are valid, false otherwise
 *
 * @throws None
 */
bool DepositCalc::FieldsValidation() {
  bool res = false;
  if (ValidateParam(ui->amount->text()) && ValidateParam(ui->term->text()) &&
      ValidateParam(ui->rate->text()) && ValidateParam(ui->term->text()) &&
      ValidateParam(ui->frequency->text())) {
    res = true;
  }
  return res;
}

/**
 * Get the replenishment amount from the UI input.
 *
 * @return the calculated replenishment amount
 *
 * @throws None
 */
double DepositCalc::GetReplen() {
  double res = 0;
  if (!ui->replen->text().isEmpty()) {
    QStringList values = ui->replen->text().split(' ');
    for (int i = 0; i < values.count(); ++i) {
      if (ValidateParam(values[i])) {
        res += values[i].toDouble();
      } else {
        res = DEPOSIT_ERROR;
        break;
      }
    }
  }
  return res;
}

/**
 * GetWithdraw function calculates the total amount to be withdrawn from the
 * deposit.
 *
 * @return the total amount to be withdrawn
 *
 * @throws None
 */
double DepositCalc::GetWithdraw() {
  double res = 0;
  if (!ui->withdraw->text().isEmpty()) {
    QStringList values = ui->withdraw->text().split(' ');
    for (int i = 0; i < values.count(); ++i) {
      if (ValidateParam(values[i])) {
        res += values[i].toDouble();
      } else {
        res = DEPOSIT_ERROR;
        break;
      }
    }
  }
  return res;
}

/**
 * Clears the output fields in the DepositCalc class.
 */
void DepositCalc::ClearFields() {
  ui->accOutput->clear();
  ui->taxOutput->clear();
  ui->DepOutput->clear();
}

/**
 * Outputs deposit data to the UI elements.
 *
 * @param data the deposit data to be displayed
 *
 * @return void
 *
 * @throws None
 */
void DepositCalc::Output(deposit_data data) {
  ui->DepOutput->setText(QString::number((double)data.dep_total, 'f', 2) +
                         " RUB");
  ui->taxOutput->setText(QString::number((double)data.tax_total, 'f', 2) +
                         " RUB");
  ui->accOutput->setText(QString::number((double)data.acc_interest, 'f', 2) +
                         " RUB");
}

/**
 * Function to handle the button press event for deposit calculation.
 */
void DepositCalc::ExecPressed() {
  ClearFields();
  deposit_data res = {0};
  if (FieldsValidation()) {
    double replen = GetReplen();
    double withdraw = GetWithdraw();
    if (replen == DEPOSIT_ERROR || withdraw == DEPOSIT_ERROR) {
      ui->DepOutput->setText("Неккоректные данные");
    } else {
      int cap_percent = ui->cap_percent->isChecked();
      res = s21_deposit_calc((double)ui->amount->text().toDouble(),
                             (double)ui->term->text().toDouble(),
                             (double)ui->rate->text().toDouble(),
                             (double)ui->tax->text().toDouble(),
                             (double)ui->frequency->text().toDouble(), replen,
                             withdraw, cap_percent);
      if (res.dep_total != DEPOSIT_ERROR) {
        Output(res);
      } else {
        ui->DepOutput->setText("Неккоректные данные");
      }
    }
  } else {
    ui->DepOutput->setText("Неккоректные данные");
  }
}
