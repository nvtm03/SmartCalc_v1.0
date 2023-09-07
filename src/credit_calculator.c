#include "credit_calculator.h"

#include <math.h>
#include <stdio.h>

void annuity_calc_credit(double total_loan_amount, double credit_term,
                         double credit_lending_rate, double *monthly_payment,
                         double *overpayment, double *total_payment) {
  credit_lending_rate /= 1200;
  *monthly_payment =
      total_loan_amount * (credit_lending_rate /
                           (1 - pow((1 + credit_lending_rate), -credit_term)));
  *overpayment = *monthly_payment * credit_term - total_loan_amount;
  *total_payment = *monthly_payment * credit_term;
}

void differentiated_calc_credit(double total_loan_amount, double credit_term,
                                double credit_lending_rate,
                                double *monthly_payments, double *overpayment,
                                double *total_payment, int *years,
                                int *months) {
  double credit_body = total_loan_amount / credit_term;
  credit_lending_rate /= 100;
  double loan_balance = total_loan_amount;
  double credit_percent;

  for (int i = 0; i < credit_term; ++i) {
    credit_percent =
        (loan_balance * credit_lending_rate * months[i]) / years[i];
    loan_balance -= credit_body;
    monthly_payments[i] = credit_percent + credit_body;
    *total_payment += monthly_payments[i];
  }
  *overpayment = *total_payment - total_loan_amount;
}
