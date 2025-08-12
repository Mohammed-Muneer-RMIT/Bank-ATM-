#include "Bank.h"
#include "Util.h"
#include <iostream>
#include <limits>

static void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void clearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void session(Bank& bank, uint64_t acc) {
    while (true) {
        std::cout << "\n=== ATM (Account " << acc << ") ===\n"
                  << "1) Balance\n"
                  << "2) Deposit\n"
                  << "3) Withdraw\n"
                  << "4) Transfer\n"
                  << "5) History\n"
                  << "6) Change PIN\n"
                  << "9) Logout\n"
                  << "Choose: ";
        int c; if (!(std::cin >> c)) { clearCin(); continue; }
        if (c == 9) break;
        if (c == 1) {
            std::cout << "Balance: $" << bank.getBalance(acc) << "\n";
        } else if (c == 2) {
            std::cout << "Amount: ";
            double a; if (!(std::cin >> a)) { clearCin(); continue; }
            if (bank.deposit(acc, a)) std::cout << "Deposited.\n"; else std::cout << "Failed.\n";
        } else if (c == 3) {
            std::cout << "Amount: ";
            double a; if (!(std::cin >> a)) { clearCin(); continue; }
            if (bank.withdraw(acc, a)) std::cout << "Withdrawn.\n"; else std::cout << "Failed.\n";
        } else if (c == 4) {
            std::cout << "To account: ";
            uint64_t to; if (!(std::cin >> to)) { clearCin(); continue; }
            std::cout << "Amount: ";
            double a; if (!(std::cin >> a)) { clearCin(); continue; }
            if (bank.transfer(acc, to, a)) std::cout << "Transferred.\n"; else std::cout << "Failed.\n";
        } else if (c == 5) {
            auto hist = bank.getHistory(acc);
            std::cout << "timestamp, type, amount, other, balance\n";
            for (auto& t : hist) {
                std::cout << t.timestamp << ", " << t.type << ", " << t.amount
                          << ", " << t.otherAccount << ", " << t.balanceAfter << "\n";
            }
        } else if (c == 6) {
            std::string np;
            std::cout << "New PIN (4-6 digits): ";
            std::cin >> np;
            if (!Util::isDigits(np) || np.size() < 4 || np.size() > 6) {
                std::cout << "Invalid PIN format.\n";
            } else if (bank.changePin(acc, np)) {
                std::cout << "PIN changed.\n";
            } else {
                std::cout << "Failed.\n";
            }
        }
    }
}

int main() {
    Bank bank("data/accounts.csv", "data/transactions.csv");
    bank.load();

    while (true) {
        std::cout << "\n=== ATM ===\n"
                  << "1) Login\n"
                  << "2) Create account\n"
                  << "0) Exit\n"
                  << "Choose: ";
        int c; if (!(std::cin >> c)) { clearCin(); continue; }
        if (c == 0) break;
        if (c == 2) {
            std::string name, pin;
            std::cout << "Your name: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, name);
            std::cout << "Choose PIN (4-6 digits): ";
            std::cin >> pin;
            if (!Util::isDigits(pin) || pin.size() < 4 || pin.size() > 6) {
                std::cout << "Invalid PIN. Try again.\n";
                continue;
            }
            auto acc = bank.createAccount(name, pin);
            std::cout << "Account created. Number: " << acc << "\n";
        } else if (c == 1) {
            uint64_t acc; std::string pin;
            std::cout << "Account number: ";
            if (!(std::cin >> acc)) { clearCin(); continue; }
            std::cout << "PIN: ";
            std::cin >> pin;
            if (bank.authenticate(acc, pin)) {
                session(bank, acc);
            } else {
                std::cout << "Invalid credentials.\n";
            }
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
