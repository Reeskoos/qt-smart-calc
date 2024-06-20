#ifndef CREDITCALC_H
#define CREDITCALC_H

#include <QWidget>

#define CREDIT_ERROR -335
extern "C" {
#include "../../calc_logic/bank_calc/include/s21_credit_calc.h"
}

namespace Ui {
class CreditCalc;
}

class CreditCalc : public QWidget {
  Q_OBJECT

 public:
  explicit CreditCalc(QWidget *parent = nullptr);
  ~CreditCalc();

 private:
  Ui::CreditCalc *ui;

 private slots:
  void ExecPressed();
  void AnnuitType(credit_data data);
  void DiffType(credit_data data);
  void InitDiffTable(credit_data data, int intTerm);
  void CleanFields();
  bool ValidateParam(QString param);
};

#endif  // CREDITCALC_H
