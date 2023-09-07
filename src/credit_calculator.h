#ifndef CREDIT_CALCULATOR_H_
#define CREDIT_CALCULATOR_H_

void annuity_calc_credit(double total_loan_amount, double credit_term,
                         double credit_lending_rate, double *monthly_payment,
                         double *overpayment, double *total_payment);
void differentiated_calc_credit(double total_loan_amount, double credit_term,
                                double credit_lending_rate,
                                double *monthly_payments, double *overpayment,
                                double *total_payment, int *years, int *months);

#endif  //  CREDIT_CALCULATOR_H_
