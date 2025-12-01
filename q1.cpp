#include <stdio.h>
#include <math.h>

double calculateRepayment(double remaining, double ratePercent, int years, double installment, int year, double *totalPaid) {
    if (years <= 0 || remaining <= 0.00001) {
        return *totalPaid;
    }
    double interest = remaining * (ratePercent / 100.0);
    remaining += interest;
    double payment = installment;
    if (payment > remaining) payment = remaining;
    remaining -= payment;
    *totalPaid += payment + 0.0;
    printf("Year %d: Remaining loan = %.2f\n", year, remaining < 0.005 ? 0.0 : remaining);
    return calculateRepayment(remaining, ratePercent, years - 1, installment, year + 1, totalPaid);
}

int main() {
    double loan = 100000.0;
    double rate = 5.0;
    int years = 3;
    double installment = loan / years;
    double totalPaid = 0.0;
    printf("Loan = %.2f, Rate = %.2f%%, Years = %d\n", loan, rate, years);
    double total = calculateRepayment(loan, rate, years, installment, 1, &totalPaid);
    double interestTotal = totalPaid - loan + fmax(0.0, loan - loan); // just show interest part approximately
    printf("Total repayment after %d years = %.2f\n", years, totalPaid);
    return 0;
}
