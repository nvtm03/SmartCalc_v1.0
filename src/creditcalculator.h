#ifndef CREDITCALCULATOR_H
#define CREDITCALCULATOR_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class CreditCalculator;
}

class CreditCalculator : public QDialog {
  Q_OBJECT

 public:
  explicit CreditCalculator(QWidget *parent = nullptr);
  ~CreditCalculator();

 private slots:
  void on_radioButton_differentiated_payments_clicked();

  void on_radioButton_annuity_payments_clicked();

  void on_pushButton_calculate_clicked();

  void on_pushButton_right_clicked();

  void on_pushButton_left_clicked();

 private:
  Ui::CreditCalculator *ui;

  int counter = 1;
  int years[360];
  int months[360];
  double monthly_payments[360];
  int credit_term;
};

#endif  // CREDITCALCULATOR_H
