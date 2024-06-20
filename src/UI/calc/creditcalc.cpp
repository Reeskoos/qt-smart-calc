/**
 * @file creditcalc.cpp
 * @brief Contains the implementation of the CreditCalc class for credit
 * calculation.
 */

#include "creditcalc.h"

#include <QDate>
#include <QStandardItemModel>

#include "ui_creditcalc.h"

/**
 * Constructor for CreditCalc class.
 *
 * @param parent pointer to the parent widget
 *
 * @return N/A
 *
 * @throws N/A
 */
CreditCalc::CreditCalc(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
  connect(ui->execButton, SIGNAL(released()), this, SLOT(ExecPressed()));
}

CreditCalc::~CreditCalc() { delete ui; }

/**
 * Validate the parameter by converting it to a double and return the result.
 *
 * @param param the parameter to be validated
 *
 * @return true if the parameter can be converted to a double, false otherwise
 *
 * @throws None
 */
bool CreditCalc::ValidateParam(QString param) {
  bool res;
  param.toDouble(&res);
  return res;
}

/**
 * Clears the fields in the CreditCalc class, including the table widget and
 * various payment fields.
 */
void CreditCalc::CleanFields() {
  ui->tableWidget->clear();
  ui->month_payment->clear();
  ui->overpayment->clear();
  ui->totalPayment->clear();
}
/**
 * Initializes the difference table for credit calculation.
 *
 * @param data the credit data
 * @param intTerm the integer term
 *
 * @return void
 *
 * @throws None
 */
void CreditCalc::InitDiffTable(credit_data data, int intTerm) {
  ui->tableWidget->setColumnCount(2);
  ui->tableWidget->setRowCount(intTerm);
  ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Дата"
                                                           << "Сумма платежа");
  for (int i = 0; i < intTerm; ++i) {
    QTableWidgetItem *item = new QTableWidgetItem(
        QString::number((double)data.diff_mouths_payment[i], 'f', 2));
    ui->tableWidget->setItem(i, 1, item);
    QTableWidgetItem *data = new QTableWidgetItem(
        QDate::currentDate().addMonths(i).toString("dd.MM.yyyy"));
    ui->tableWidget->setItem(i, 0, data);
  }
}

/**
 * Updates the UI with the calculated monthly payment, overpayment, and total
 * payment.
 *
 * @param data the credit data used for calculations
 *
 * @return void
 *
 * @throws None
 */
void CreditCalc::AnnuitType(credit_data data) {
  double dblOverPayment = data.overpayment;
  double dblTotalPayment = data.total_payment;
  double dblMountPayment = data.monthly_payment;
  ui->month_payment->setText(QString::number(dblMountPayment, 'f', 2) + " RUB");
  ui->overpayment->setText(QString::number(dblOverPayment, 'f', 2) + " RUB");
  ui->totalPayment->setText(QString::number(dblTotalPayment, 'f', 2) + " RUB");
}

/**
 * Calculate and display different types of credit payments.
 *
 * @param data credit data containing overpayment, total payment, and terms
 *
 * @return void
 *
 * @throws None
 */
void CreditCalc::DiffType(credit_data data) {
  double dblOverPayment = data.overpayment;
  double dblTotalPayment = data.total_payment;
  int intTerm = ui->term->text().toInt();

  ui->month_payment->setText(
      "От " + QString::number((double)data.diff_mouths_payment[0], 'f', 2) +
      " RUB\nДо " +
      QString::number((double)data.diff_mouths_payment[intTerm - 1], 'f', 2) +
      " RUB");
  ui->overpayment->setText(QString::number(dblOverPayment, 'f', 2) + " RUB");
  ui->totalPayment->setText(QString::number(dblTotalPayment, 'f', 2) + " RUB");
  InitDiffTable(data, intTerm);
}

/**
 * Executes the CreditCalc function, cleaning fields and validating parameters.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void CreditCalc::ExecPressed() {
  CleanFields();
  if (ValidateParam(ui->amount->text()) && ValidateParam(ui->term->text()) &&
      ValidateParam(ui->rate->text())) {
    credit_data data = {0};
    data = s21_credit_calc(
        ui->amount->text().toDouble(), ui->term->text().toDouble(),
        ui->rate->text().toDouble(), ui->type->currentIndex());

    if (data.total_payment != CREDIT_ERROR) {
      if (ui->type->currentIndex() == 0) {
        AnnuitType(data);
      } else if (ui->type->currentIndex() == 1) {
        DiffType(data);
      }
    } else {
      ui->totalPayment->setText("Некорректные данные");
    }
  } else {
    ui->totalPayment->setText("Некорректные данные");
  }
}
