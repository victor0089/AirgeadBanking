#include "InvestmentCalculator.h"

#include <cmath>
#include <stdexcept>

namespace {
const int MONTHS_PER_YEAR = 12;
const int MAX_YEARS = 100;
const double PERCENT_CONVERSION = 100.0;
}

InvestmentCalculator::InvestmentCalculator(double t_initialInvestment,
                                           double t_monthlyDeposit,
                                           double t_annualInterestRate,
                                           int t_numberOfYears)
    : m_initialInvestment(t_initialInvestment),
      m_monthlyDeposit(t_monthlyDeposit),
      m_annualInterestRate(t_annualInterestRate),
      m_numberOfYears(t_numberOfYears) {
    // Validate here as well as in the user interface, so the class is safe to reuse.
    if (!std::isfinite(m_initialInvestment) || m_initialInvestment <= 0.0) {
        throw std::invalid_argument("Initial investment must be greater than zero.");
    }
    if (!std::isfinite(m_monthlyDeposit) || m_monthlyDeposit < 0.0) {
        throw std::invalid_argument("Monthly deposit cannot be negative.");
    }
    if (!std::isfinite(m_annualInterestRate) || m_annualInterestRate < 0.0) {
        throw std::invalid_argument("Annual interest cannot be negative.");
    }
    if (m_numberOfYears < 1 || m_numberOfYears > MAX_YEARS) {
        throw std::invalid_argument("Number of years must be between 1 and 100.");
    }
}

std::vector<YearlySummary> InvestmentCalculator::calculateYearlyReport(
    bool t_includeMonthlyDeposits) const {
    std::vector<YearlySummary> report;
    report.reserve(m_numberOfYears);

    double openingBalance = m_initialInvestment;
    const double depositedAmount = t_includeMonthlyDeposits ? m_monthlyDeposit : 0.0;
    const double monthlyInterestRate =
        (m_annualInterestRate / PERCENT_CONVERSION) / MONTHS_PER_YEAR;

    for (int year = 1; year <= m_numberOfYears; ++year) {
        double yearEndInterest = 0.0;

        for (int month = 1; month <= MONTHS_PER_YEAR; ++month) {
            // Airgead's required ordering: add this month's deposit BEFORE interest.
            const double total = openingBalance + depositedAmount;
            const double monthlyInterest = total * monthlyInterestRate;
            const double closingBalance = total + monthlyInterest;

            // Stop cleanly if the values are too large to represent safely.
            if (!std::isfinite(total) || !std::isfinite(monthlyInterest) ||
                !std::isfinite(closingBalance)) {
                throw std::overflow_error(
                    "The investment is too large to calculate accurately.");
            }

            yearEndInterest += monthlyInterest;
            openingBalance = closingBalance;
        }

        if (!std::isfinite(yearEndInterest)) {
            throw std::overflow_error("The annual interest is too large.");
        }

        // Keep the unrounded balance for the following year; only the UI rounds.
        YearlySummary summary = {year, openingBalance, yearEndInterest};
        report.push_back(summary);
    }

    return report;
}
