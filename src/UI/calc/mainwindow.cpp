/**
 * @file mainwindow.cpp
 * @brief Contains the implementation of the MainWindow class for calc UI.
 */

#include "mainwindow.h"

#include "./ui_mainwindow.h"
extern "C" {
#include "../../calc_logic/s21_calc.h"
}
#include "creditcalc.h"
#include "depositcalc.h"

QString(expression);

/**
 * Constructor for the MainWindow class.
 *
 * @param parent pointer to the parent widget
 *
 * @return void
 *
 * @throws None
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->display->setText("0");
  QPushButton *numButtons[11] = {0};
  for (int i = 0; i < 11; ++i) {
    QString buttonName = "Button" + QString::number(i);
    numButtons[i] = MainWindow::findChild<QPushButton *>(buttonName);
    connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
  }
  connect(ui->plus, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->minus, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->div, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->mult, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->power, SIGNAL(released()), this, SLOT(OperPressed()));

  connect(ui->left_bracket, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->right_bracket, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->cos, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->sin, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->tan, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->acos, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->asin, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->atan, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->mod, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->sqrt, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->log, SIGNAL(released()), this, SLOT(OperPressed()));
  connect(ui->ln, SIGNAL(released()), this, SLOT(OperPressed()));

  connect(ui->result, SIGNAL(released()), this, SLOT(EqualButton()));
  connect(ui->clear, SIGNAL(released()), this, SLOT(CancelPressed()));

  connect(ui->menuCredit, SIGNAL(triggered()), this, SLOT(CreditPressed()));
  connect(ui->menuDeposit, SIGNAL(triggered()), this, SLOT(DepositPressed()));
}

MainWindow::~MainWindow() { delete ui; }

/**
 * Handles the button press event and updates the display accordingly.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void MainWindow::NumPressed() {
  QPushButton *button = (QPushButton *)sender();
  QString buttonValue = button->text();
  if (expression.size() == 0) {
    ui->display->setText(buttonValue);
    expression.append(buttonValue);
  } else {
    expression.append(buttonValue);
    ui->display->setText(expression);
  }
}

/**
 * Handle the operation button press in the main window.
 */
void MainWindow::OperPressed() {
  QPushButton *button = (QPushButton *)sender();
  expression.append(button->text().toLower());
  ui->display->setText(expression);
}

/**
 * The EqualButton function processes the input expression and displays the
 * result on the UI.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void MainWindow::EqualButton() {
  long double res = 0.0;
  std::string cppExpr(expression.toStdString());
  if (cppExpr.length() > 255) {
    ValidationError(1569325044);
  } else {
    res = s21_smart_calc(cppExpr.c_str());
    if (res == 1569325041 || res == 1569325042 || res == 1569325043) {
      ValidationError(res);
    } else {
      ui->display->setText(QString::number((double)res, 'f', 7));
    }
  }
  expression.clear();
}

/**
 * Function to handle the cancel button press in the MainWindow.
 */
void MainWindow::CancelPressed() {
  expression.clear();
  ui->display->setText("");
}

/**
 * Handle validation errors and display appropriate error message on the UI.
 *
 * @param res the code representing the specific validation error
 *
 * @return void
 *
 * @throws None
 */
void MainWindow::ValidationError(long double res) {
  QString error;
  if (res == 1569325041) error = "BRACKETS DO NOT MATCH";
  if (res == 1569325042) error = "INVALID EXPRESSION";
  if (res == 1569325043) error = "UNKNOWN FUNC";
  if (res == 1569325044) error = "CHARACTER LIMIT REACHED (max 255)";
  ui->display->setText(error);
}

/**
 * CreditPressed function creates a new CreditCalc object and shows the credit
 * window.
 */
void MainWindow::CreditPressed() {
  CreditCalc *creditWindow = new CreditCalc;
  creditWindow->show();
}

/**
 * DepositPressed function creates a new DepositCalc object and shows the
 * deposit window.
 */
void MainWindow::DepositPressed() {
  DepositCalc *depositWindow = new DepositCalc;
  depositWindow->show();
}
