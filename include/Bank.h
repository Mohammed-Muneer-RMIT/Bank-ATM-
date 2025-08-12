#pragma once
#include "Account.h"
#include <unordered_map>
#include <string>
#include <vector>

struct Txn {
    std::string timestamp;
    uint64_t account;
    std::string type;      // deposit/withdraw/transfer-in/transfer-out
    double amount;
    uint64_t otherAccount; // 0 if none
    double balanceAfter;
};

class Bank {
public:
    Bank(const std::string& accountsCsv, const std::string& txnsCsv);
    void load();
    void save();

    uint64_t createAccount(const std::string& name, const std::string& pin);
    bool authenticate(uint64_t number, const std::string& pin);

    double getBalance(uint64_t number) const;
    bool deposit(uint64_t number, double amount);
    bool withdraw(uint64_t number, double amount);
    bool transfer(uint64_t from, uint64_t to, double amount);

    std::vector<Txn> getHistory(uint64_t number) const;
    bool changePin(uint64_t number, const std::string& newPin);

private:
    std::unordered_map<uint64_t, Account> accounts_;
    std::vector<Txn> transactions_;
    std::string accountsCsv_;
    std::string txnsCsv_;
    const std::string salt_ = "atm_demo_salt";

    uint64_t nextAccountNumber() const;
    void record(uint64_t acc, const std::string& type, double amount, uint64_t otherAcc);
};
