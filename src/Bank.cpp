#include "Bank.h"
#include "Util.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

Bank::Bank(const std::string& accountsCsv, const std::string& txnsCsv)
    : accountsCsv_(accountsCsv), txnsCsv_(txnsCsv) {}

void Bank::load() {
    accounts_.clear();
    transactions_.clear();

    std::ifstream fa(accountsCsv_);
    std::string line;
    // Skip header if present
    while (std::getline(fa, line)) {
        if (line.empty()) continue;
        if (line.rfind("accountNumber", 0) == 0) continue;
        std::stringstream ss(line);
        std::string number, name, pinHash, bal;
        std::getline(ss, number, ',');
        std::getline(ss, name, ',');
        std::getline(ss, pinHash, ',');
        std::getline(ss, bal, ',');
        try {
            uint64_t n = std::stoull(number);
            double b = std::stod(bal);
            accounts_.emplace(n, Account{n, name, pinHash, b});
        } catch (...) {}
    }

    std::ifstream ft(txnsCsv_);
    while (std::getline(ft, line)) {
        if (line.empty()) continue;
        if (line.rfind("timestamp", 0) == 0) continue;
        std::stringstream ss(line);
        Txn t;
        std::string acc, amt, other, bal;
        std::getline(ss, t.timestamp, ',');
        std::getline(ss, acc, ',');
        std::getline(ss, t.type, ',');
        std::getline(ss, amt, ',');
        std::getline(ss, other, ',');
        std::getline(ss, bal, ',');
        try {
            t.account = std::stoull(acc);
            t.amount = std::stod(amt);
            t.otherAccount = std::stoull(other);
            t.balanceAfter = std::stod(bal);
            transactions_.push_back(t);
        } catch (...) {}
    }
}

void Bank::save() {
    std::ofstream fa(accountsCsv_);
    fa << "accountNumber,name,pinHash,balance\n";
    for (auto& [n, a] : accounts_) {
        fa << n << "," << a.name << "," << a.pinHash << "," << std::fixed << std::setprecision(2) << a.balance << "\n";
    }

    std::ofstream ft(txnsCsv_);
    ft << "timestamp,accountNumber,type,amount,otherAccount,balanceAfter\n";
    for (auto& t : transactions_) {
        ft << t.timestamp << "," << t.account << "," << t.type << ","
           << std::fixed << std::setprecision(2) << t.amount << ","
           << t.otherAccount << "," << t.balanceAfter << "\n";
    }
}

uint64_t Bank::nextAccountNumber() const {
    uint64_t maxn = 100000;
    for (auto& kv : accounts_) {
        if (kv.first > maxn) maxn = kv.first;
    }
    return maxn + 1;
}

uint64_t Bank::createAccount(const std::string& name, const std::string& pin) {
    uint64_t number = nextAccountNumber();
    auto ph = Util::hashPin(pin, salt_);
    accounts_.emplace(number, Account{number, name, ph, 0.0});
    record(number, "open", 0.0, 0);
    save();
    return number;
}

bool Bank::authenticate(uint64_t number, const std::string& pin) {
    auto it = accounts_.find(number);
    if (it == accounts_.end()) return false;
    return it->second.pinHash == Util::hashPin(pin, salt_);
}

double Bank::getBalance(uint64_t number) const {
    auto it = accounts_.find(number);
    if (it == accounts_.end()) return 0.0;
    return it->second.balance;
}

bool Bank::deposit(uint64_t number, double amount) {
    if (amount <= 0) return false;
    auto it = accounts_.find(number);
    if (it == accounts_.end()) return false;
    it->second.balance += amount;
    record(number, "deposit", amount, 0);
    save();
    return true;
}

bool Bank::withdraw(uint64_t number, double amount) {
    if (amount <= 0) return false;
    auto it = accounts_.find(number);
    if (it == accounts_.end()) return false;
    if (it->second.balance < amount) return false;
    it->second.balance -= amount;
    record(number, "withdraw", amount, 0);
    save();
    return true;
}

bool Bank::transfer(uint64_t from, uint64_t to, double amount) {
    if (amount <= 0) return false;
    auto itf = accounts_.find(from);
    auto itt = accounts_.find(to);
    if (itf == accounts_.end() || itt == accounts_.end()) return false;
    if (itf->second.balance < amount) return false;
    itf->second.balance -= amount;
    itt->second.balance += amount;
    record(from, "transfer-out", amount, to);
    record(to, "transfer-in", amount, from);
    save();
    return true;
}

std::vector<Txn> Bank::getHistory(uint64_t number) const {
    std::vector<Txn> out;
    for (auto& t : transactions_) {
        if (t.account == number) out.push_back(t);
    }
    return out;
}

bool Bank::changePin(uint64_t number, const std::string& newPin) {
    auto it = accounts_.find(number);
    if (it == accounts_.end()) return false;
    it->second.pinHash = Util::hashPin(newPin, salt_);
    record(number, "pin-change", 0.0, 0);
    save();
    return true;
}

void Bank::record(uint64_t acc, const std::string& type, double amount, uint64_t otherAcc) {
    Txn t;
    t.timestamp = Util::nowTimestamp();
    t.account = acc;
    t.type = type;
    t.amount = amount;
    t.otherAccount = otherAcc;
    t.balanceAfter = accounts_[acc].balance;
    transactions_.push_back(t);
}
