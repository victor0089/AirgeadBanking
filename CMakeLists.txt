cmake_minimum_required(VERSION 3.5)
project(AirgeadBanking CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AirgeadBanking
    Main.cpp
    BankingApp.cpp
    BankingApp.h
    InvestmentCalculator.cpp
    InvestmentCalculator.h
)

enable_testing()
add_executable(InvestmentTests
    tests/InvestmentCalculatorTests.cpp
    InvestmentCalculator.cpp
    InvestmentCalculator.h
)
add_test(NAME InvestmentTests COMMAND InvestmentTests)
