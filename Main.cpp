#include "BankingApp.h"

#include <exception>
#include <iostream>

int main() {
    try {
        // Keep main small: the application object owns all program behavior.
        BankingApp app;
        app.run();
    } catch (const std::exception& error) {
        // Report an unexpected input or arithmetic problem instead of crashing.
        std::cerr << "\nUnable to complete calculation: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
