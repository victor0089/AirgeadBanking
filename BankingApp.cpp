#include "BankingApp.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
const int MAX_YEARS = 100;
const int TABLE_WIDTH = 73;

// Use one entire line for each entry, so failed extraction cannot leave junk
// in the input stream or accidentally skip a later prompt.
std::string readInputLine() {
    std::string input;
    if (!std::getline(std::cin, input)) {
        throw std::runtime_error("Input ended before the application finished.");
    }
    return input;
}
}

void BankingApp::displayInputHeader() const {
    std::cout << '\n' << std::string(73, '=') << '\n';
    std::cout << "                  AIRGEAD BANKING | INVESTMENT PLANNER\n";
    std::cout << std::string(73, '=') << '\n';
    std::cout << "Enter your investment details to compare compound-interest growth.\n";
    std::cout << "(Use numbers without $ or % symbols.)\n\n";
}

double BankingApp::readMoneyOrRate(const std::string& t_prompt,
                                    bool t_requirePositive) const {
    while (true) {
        std::cout << t_prompt;
        const std::string input = readInputLine();
        std::istringstream parser(input);
        double amount = 0.0;
        char extra = '\0';

        // Reject blank entries, nonnumeric text, trailing junk, NaN, and infinity.
        if (!(parser >> amount) || (parser >> extra) || !std::isfinite(amount)) {
            std::cout << "  Invalid entry. Enter a valid number (example: 50.00).\n";
            continue;
        }

        if ((t_requirePositive && amount <= 0.0) ||
            (!t_requirePositive && amount < 0.0)) {
            std::cout << (t_requirePositive
                              ? "  Please enter a number greater than zero.\n"
                              : "  Please enter zero or a positive number.\n");
            continue;
        }
        return amount;
    }
}

int BankingApp::readNumberOfYears() const {
    while (true) {
        std::cout << "Number of Years (1-100): ";
        const std::string input = readInputLine();
        std::istringstream parser(input);
        int years = 0;
        char extra = '\0';

        // Whole years only; the bounded range avoids enormous report output.
        if (!(parser >> years) || (parser >> extra) ||
            years < 1 || years > MAX_YEARS) {
            std::cout << "  Invalid entry. Enter a whole number from 1 to 100.\n";
            continue;
        }
        return years;
    }
}

void BankingApp::waitForEnter() const {
    std::cout << "\nPress Enter to continue and view both reports...";
    readInputLine();
}

std::string BankingApp::formatCurrency(double t_amount) const {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << t_amount;
    std::string amount = output.str();

    // Insert thousands separators for easier comparison of larger balances.
    const std::string::size_type decimal = amount.find('.');
    const std::string::size_type wholeLength =
        (decimal == std::string::npos) ? amount.size() : decimal;
    for (std::string::size_type position = wholeLength; position > 3; position -= 3) {
        amount.insert(position - 3, ",");
    }
    return "$" + amount;
}

void BankingApp::displayYearlyReport(
    const std::string& t_title,
    const std::vector<YearlySummary>& t_report) const {
    std::cout << '\n' << std::string(TABLE_WIDTH, '=') << '\n';
    std::cout << t_title << '\n';
    std::cout << std::string(TABLE_WIDTH, '=') << '\n';
    std::cout << std::left << std::setw(9) << "Year"
              << std::right << std::setw(28) << "Year End Balance"
              << std::setw(36) << "Year End Earned Interest" << '\n';
    std::cout << std::string(TABLE_WIDTH, '-') << '\n';

    for (std::vector<YearlySummary>::const_iterator row = t_report.begin();
         row != t_report.end(); ++row) {
        std::cout << std::left << std::setw(9) << row->year
                  << std::right << std::setw(28)
                  << formatCurrency(row->yearEndBalance)
                  << std::setw(36) << formatCurrency(row->yearEndInterest)
                  << '\n';
    }
}

bool BankingApp::promptToTryAgain() const {
    while (true) {
        std::cout << "\nWould you like to try different investment values? (Y/N): ";
        const std::string response = readInputLine();
        if (response == "Y" || response == "y" ||
            response == "YES" || response == "Yes" || response == "yes") {
            return true;
        }
        if (response == "N" || response == "n" ||
            response == "NO" || response == "No" || response == "no") {
            return false;
        }
        std::cout << "  Please enter Y or N.\n";
    }
}

void BankingApp::run() const {
    bool runAgain = true;

    while (runAgain) {
        displayInputHeader();
        const double initialInvestment =
            readMoneyOrRate("Initial Investment Amount ($): ", true);
        // Zero is useful for comparing growth without contributing more money.
        const double monthlyDeposit =
            readMoneyOrRate("Monthly Deposit ($): ", false);
        const double annualInterestRate =
            readMoneyOrRate("Annual Interest (%): ", false);
        const int numberOfYears = readNumberOfYears();

        InvestmentCalculator investment(initialInvestment, monthlyDeposit,
                                        annualInterestRate, numberOfYears);
        waitForEnter();

        // Both required reports start from exactly the same initial investment.
        displayYearlyReport("Balance and Interest WITHOUT Additional Monthly Deposits",
                            investment.calculateYearlyReport(false));
        displayYearlyReport("Balance and Interest WITH Additional Monthly Deposits",
                            investment.calculateYearlyReport(true));

        runAgain = promptToTryAgain();
    }

    std::cout << "\nThank you for using Airgead Banking's Investment Planner!\n";
}
