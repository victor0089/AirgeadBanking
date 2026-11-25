# Airgead Banking - CS 210 Project Two

An object-oriented, interactive C++ console application that demonstrates monthly compound interest. The application presents **both required year-end reports**: with and without added monthly deposits.

## Visual Studio : open and run  

1. Extract all files from `AirgeadBanking\_ProjectTwo.zip` into a folder.
2. Open `AirgeadBanking.sln` in **Visual Studio 2015**.
3. Choose `Debug` and either `Win32` or `x64` from the toolbar.
4. Select **Build > Build Solution**.
5. Select **Debug > Start Without Debugging** (`Ctrl+F5`).
6. Enter the investment amount, monthly deposit, interest rate, and years.
7. Press **Enter** to show both reports; choose **Y** to compare new values.

No third-party libraries, network access, or Windows-specific commands are needed. Source code uses C++11 features supported by Visual Studio 2015.

## Other compilers / VS Code

Compile from the extracted folder with any C++11-capable GCC or Clang:

```sh
g++ -std=c++11 -Wall -Wextra -pedantic Main.cpp BankingApp.cpp InvestmentCalculator.cpp -o AirgeadBanking
./AirgeadBanking
```

Alternatively, use `CMakeLists.txt` with your configured compiler.

## Test with the official Airgead example

Enter these values:

|Field|Example|
|-|-:|
|Initial Investment|$1.00|
|Monthly Deposit|$50.00|
|Annual Interest|5%|
|Number of Years|5|

The year-five balance **with deposits** should be **$3,415.76**, and the interest earned during year five should be **$152.75**. The year-five balance **without deposits** should be **$1.28**.

## Input rules

* Initial investment: a finite number greater than zero.
* Monthly deposit: a finite number greater than or equal to zero. Zero deliberately allows a no-deposit scenario.
* Annual interest: a finite percent greater than or equal to zero. Zero demonstrates growth without interest.
* Years: a whole number from 1 to 100 to keep reports manageable.
* Invalid entries prompt the user to re-enter a value instead of crashing.

## How interest is calculated

Each month: `total = openingBalance + monthlyDeposit`, followed by `monthlyInterest = total \* (annualInterestRate / 100 / 12)` and `closingBalance = total + monthlyInterest`. Each year's interest is the sum of its 12 monthly interest amounts. The math retains full floating-point precision; the application formats values to two decimal places only in the console report, consistent with the Airgead example.

This is an educational planning tool, not a production banking transaction system. Real financial systems usually represent currency with a fixed-point decimal or integer cents type and a specified rounding policy.

## Project files

* `Main.cpp` - minimal entry point and exception handling.
* `BankingApp.h/.cpp` - input validation, user prompts, and reports.
* `InvestmentCalculator.h/.cpp` - reusable calculation class.
* `AirgeadBanking.sln/.vcxproj/.filters` - ready-to-open VS2015 solution.
* `CMakeLists.txt` - optional cross-platform build and tests.
* `tests/InvestmentCalculatorTests.cpp` - automated checks, including the provided example.
* `Pseudocode.txt` - the required step-by-step program plan.

To run the tests with GCC/Clang:

```sh
g++ -std=c++11 -Wall -Wextra -pedantic tests/InvestmentCalculatorTests.cpp InvestmentCalculator.cpp -o InvestmentTests
./InvestmentTests
```

## Submission

Upload the completed ZIP folder and the separate pseudocode document as requested by your CS 210 instructor. The ZIP already includes the pseudocode text as an extra convenience.

