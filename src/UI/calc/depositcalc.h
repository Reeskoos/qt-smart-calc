#ifndef DEPOSITCALC_H
#define DEPOSITCALC_H

#include <QWidget>

extern "C" {
#include "../../calc_logic/bank_calc/include/s21_deposit_calc.h"
}

namespace Ui {
class DepositCalc;
}

class DepositCalc : public QWidget {
  Q_OBJECT

 public:
  explicit DepositCalc(QWidget *parent = nullptr);
  ~DepositCalc();

 private:
  Ui::DepositCalc *ui;

 private slots:
  void ExecPressed();
  void Output(deposit_data data);
  double GetReplen();
  double GetWithdraw();
  bool FieldsValidation();
  bool ValidateParam(QString param);
  void ClearFields();
};

#endif  // DEPOSITCALC_H
