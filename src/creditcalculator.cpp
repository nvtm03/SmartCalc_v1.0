#include "creditcalculator.h"

#include <QDateTime>

#include "ui_creditcalculator.h"

extern "C" {
#include "credit_calculator.h"
}

CreditCalculator::CreditCalculator(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreditCalculator) {
  ui->setupUi(this);
  ui->label->show();

  if (ui->radioButton_differentiated_payments->isChecked())
    ui->month_number->hide();
  ui->pushButton_left->hide();
  ui->pushButton_right->hide();
  ui->label_monthly_payment_diff->hide();
}

CreditCalculator::~CreditCalculator() { delete ui; }

void CreditCalculator::on_radioButton_differentiated_payments_clicked() {
  ui->month_number->setVisible(true);
  ui->pushButton_left->setVisible(true);
  ui->pushButton_right->setVisible(true);
  ui->label_monthly_payment_diff->setVisible(true);
  ui->label_monthly_payment->hide();
  ui->monthly_payment->hide();
  ui->overpayment->clear();
  ui->total_payment->clear();
}

void CreditCalculator::on_radioButton_annuity_payments_clicked() {
  ui->month_number->hide();
  ui->pushButton_left->hide();
  ui->pushButton_right->hide();
  ui->label_monthly_payment_diff->hide();
  ui->label_monthly_payment->show();
  ui->monthly_payment->show();
  ui->overpayment->clear();
  ui->total_payment->clear();
}

void CreditCalculator::on_pushButton_calculate_clicked() {
  double total_loan_amount = ui->total_loan_amount->text().toDouble();
  credit_term = ui->credit_term->text().toDouble();
  double credit_lending_rate = ui->credit_lending_rate->text().toDouble();
  double overpayment = 0.0;
  double total_payment = 0.0;
  if (total_loan_amount != 0 && credit_term != 0 && credit_lending_rate != 0) {
    if (ui->radioButton_differentiated_payments->isChecked()) {
      QDateTime dt = QDateTime::currentDateTime();
      int current_month = dt.date().month();
      int current_year = dt.date().year();
      int current_day = 1;
      QDate current;

      for (int i = 0; i < credit_term; ++i) {
        current.setDate(current_year, current_month, current_day);
        years[i] = current.daysInYear();
        months[i] = current.daysInMonth();
        if (current_month == 12) {
          current_month = 1;
          current_year += 1;
        } else {
          current_month += 1;
        }
      }
      differentiated_calc_credit(total_loan_amount, credit_term,
                                 credit_lending_rate, monthly_payments,
                                 &overpayment, &total_payment, years, months);
      ui->monthly_payment->clear();
      ui->total_payment->setText(
          QString::number(round(total_payment), 'g', 15));
      ui->overpayment->setText(QString::number(round(overpayment), 'g', 15));
      ui->month_number->setText("Платёж №" + QString::number(counter));
      ui->label_monthly_payment_diff->setText(
          QString::number(round(monthly_payments[counter - 1]), 'g', 15));
    } else {
      double monthly_payment = 0.0;
      annuity_calc_credit(total_loan_amount, credit_term, credit_lending_rate,
                          &monthly_payment, &overpayment, &total_payment);
      ui->overpayment->setText(QString::number(round(overpayment), 'g', 15));
      ui->total_payment->setText(
          QString::number(round(total_payment), 'g', 15));
      ui->monthly_payment->setText(
          QString::number(round(monthly_payment), 'g', 15));
    }
  }
}

void CreditCalculator::on_pushButton_right_clicked() {
  if (counter + 1 <= credit_term) {
    ++counter;
  } else {
    counter = 1;
  }
  ui->month_number->setText("Платёж №" + QString::number(counter));
  ui->label_monthly_payment_diff->setText(
      QString::number(round(monthly_payments[counter - 1]), 'g', 15));
}

void CreditCalculator::on_pushButton_left_clicked() {
  if (counter - 1 >= 1) {
    --counter;
  } else {
    counter = credit_term;
  }
  ui->month_number->setText("Платёж №" + QString::number(counter));
  ui->label_monthly_payment_diff->setText(
      QString::number(round(monthly_payments[counter - 1]), 'g', 15));
}
