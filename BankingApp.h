#ifndef AIRGEAD_BANKING_BANKING_APP_H_
#define AIRGEAD_BANKING_BANKING_APP_H_

#include "InvestmentCalculator.h"

#include <string>
#include <vector>

// Handles all console input and output, separate from investment mathematics.
class BankingApp {
public:
    void run() const;

private:
    void displayInputHeader() const;
    double readMoneyOrRate(const std::string& t_prompt,
                           bool t_requirePositive) const;
    int readNumberOfYears() const;
    void waitForEnter() const;
    void displayYearlyReport(const std::string& t_title,
                             const std::vector<YearlySummary>& t_report) const;
    bool promptToTryAgain() const;
    std::string formatCurrency(double t_amount) const;
};

#endif // AIRGEAD_BANKING_BANKING_APP_H_
