#pragma once
#include <string>
#include <cstdint>

struct Account {
    uint64_t number;
    std::string name;
    std::string pinHash; // salted
    double balance;

    Account() : number(0), balance(0.0) {}
    Account(uint64_t n, std::string nm, std::string ph, double bal)
        : number(n), name(std::move(nm)), pinHash(std::move(ph)), balance(bal) {}
};
