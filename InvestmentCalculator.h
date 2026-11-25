#ifndef AIRGEAD_BANKING_INVESTMENT_CALCULATOR_H_
#define AIRGEAD_BANKING_INVESTMENT_CALCULATOR_H_

#include <vector>

// Describes the results for one completed year of an investment.
struct YearlySummary {
    int year;
    double yearEndBalance;
    double yearEndInterest;
};

// Calculates the month-by-month growth of an investment.
// Financial figures retain their precision until they are displayed.
class InvestmentCalculator {
public:
    InvestmentCalculator(double t_initialInvestment,
                         double t_monthlyDeposit,
                         double t_annualInterestRate,
                         int t_numberOfYears);

    // Pass false for the no-deposit comparison and true for the entered deposit.
    std::vector<YearlySummary> calculateYearlyReport(
        bool t_includeMonthlyDeposits) const;

private:
    double m_initialInvestment;
    double m_monthlyDeposit;
    double m_annualInterestRate;
    int m_numberOfYears;
};

#endif // AIRGEAD_BANKING_INVESTMENT_CALCULATOR_H_
