#include "../InvestmentCalculator.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
// Compare amounts at the same two-decimal display precision as the rubric.
void expectCurrency(double t_actual, double t_expected, const char* t_description) {
    if (!std::isfinite(t_actual) ||
        std::floor(t_actual * 100.0 + 0.5) !=
        std::floor(t_expected * 100.0 + 0.5)) {
        std::cerr << "FAILED: " << t_description << " (actual " << t_actual
                  << ", expected " << t_expected << ")\n";
        throw std::runtime_error("Calculation failed the expected result.");
    }
}

void expect(bool t_condition, const char* t_description) {
    if (!t_condition) {
        throw std::runtime_error(t_description);
    }
}
}

int main() {
    try {
        InvestmentCalculator sample(1.00, 50.00, 5.00, 5);
        const std::vector<YearlySummary> withDeposits =
            sample.calculateYearlyReport(true);
        const std::vector<YearlySummary> withoutDeposits =
            sample.calculateYearlyReport(false);

        const double expectedBalances[] = {
            617.55, 1265.65, 1946.90, 2663.01, 3415.76
        };
        const double expectedInterest[] = {
            16.55, 48.10, 81.25, 116.11, 152.75
        };
        for (int index = 0; index < 5; ++index) {
            expect(withDeposits[index].year == index + 1, "Year numbers must be sequential.");
            expectCurrency(withDeposits[index].yearEndBalance,
                           expectedBalances[index], "Deposit scenario balance");
            expectCurrency(withDeposits[index].yearEndInterest,
                           expectedInterest[index], "Deposit scenario interest");
        }
        expect(withoutDeposits.size() == 5, "No-deposit report must contain five years.");
        expectCurrency(withoutDeposits[0].yearEndBalance, 1.05,
                       "No-deposit first year balance");
        expectCurrency(withoutDeposits[4].yearEndBalance, 1.28,
                       "No-deposit fifth year balance");

        InvestmentCalculator zeroInterest(100.00, 25.00, 0.0, 2);
        const std::vector<YearlySummary> flat =
            zeroInterest.calculateYearlyReport(true);
        expectCurrency(flat[0].yearEndBalance, 400.00, "Zero interest first year");
        expectCurrency(flat[1].yearEndBalance, 700.00, "Zero interest second year");
        expectCurrency(flat[1].yearEndInterest, 0.00, "Zero earned interest");

        InvestmentCalculator zeroDeposit(200.00, 0.0, 6.0, 1);
        const std::vector<YearlySummary> sameA =
            zeroDeposit.calculateYearlyReport(false);
        const std::vector<YearlySummary> sameB =
            zeroDeposit.calculateYearlyReport(true);
        expectCurrency(sameA[0].yearEndBalance, sameB[0].yearEndBalance,
                       "Zero-deposit scenarios must match");

        bool invalidRejected = false;
        try {
            InvestmentCalculator invalid(-1.0, 50.0, 5.0, 5);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Negative initial investment must be rejected.");

        invalidRejected = false;
        try {
            InvestmentCalculator invalid(1.0, -50.0, 5.0, 5);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Negative monthly deposit must be rejected.");

        invalidRejected = false;
        try {
            InvestmentCalculator invalid(1.0, 50.0, -5.0, 5);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Negative interest must be rejected.");

        invalidRejected = false;
        try {
            InvestmentCalculator invalid(1.0, 50.0, 5.0, 0);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Zero years must be rejected.");

        invalidRejected = false;
        try {
            InvestmentCalculator invalid(1.0, 50.0, 5.0, 101);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Years above 100 must be rejected.");

        invalidRejected = false;
        try {
            InvestmentCalculator invalid(
                std::numeric_limits<double>::infinity(), 50.0, 5.0, 5);
        } catch (const std::invalid_argument&) {
            invalidRejected = true;
        }
        expect(invalidRejected, "Infinite input must be rejected.");

        std::cout << "All investment calculator tests passed.\n";
    } catch (const std::exception& error) {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
