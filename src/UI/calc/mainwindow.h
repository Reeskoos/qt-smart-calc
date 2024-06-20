#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

 private slots:
  void NumPressed();
  void OperPressed();
  void EqualButton();
  void CancelPressed();
  void ValidationError(long double res);
  void CreditPressed();
  void DepositPressed();
};
#endif  // MAINWINDOW_H
