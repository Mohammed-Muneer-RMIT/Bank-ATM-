ATM Simulator (C++)

A simple, console-based ATM simulator written in modern C++.
It supports account creation, login with PIN, balance checking, deposit, withdraw,
transfer, and transaction history with CSV persistence.

------------------------------------------------------------
FEATURES
------------------------------------------------------------
- Create account (unique account number auto-assigned)
- Login via account number + PIN
- View balance
- Deposit / Withdraw with validation
- Transfer between accounts
- Transaction log per account (CSV)
- Persistent storage in data/accounts.csv and data/transactions.csv
- Simple CLI
- Makefile build

------------------------------------------------------------
BUILD & RUN
------------------------------------------------------------
Option A — Using Makefile
    1. Open terminal in the project folder.
    2. Type: make
    3. Run with: ./atm   (Linux/Mac)
       or: .\atm.exe    (Windows)

Option B — Manual compile without Makefile
    g++ -std=c++17 -O2 -Wall src/*.cpp -Iinclude -o atm
    ./atm

------------------------------------------------------------
SAMPLE ACCOUNTS
------------------------------------------------------------
- Account: 100001 | PIN: 1234 | Balance: 500.00
- Account: 100002 | PIN: 4321 | Balance: 1200.00

------------------------------------------------------------
CLI OVERVIEW
------------------------------------------------------------
Main Menu:
    1) Login
    2) Create account
    0) Exit

Once logged in:
    1) Balance
    2) Deposit
    3) Withdraw
    4) Transfer
    5) History
    6) Change PIN
    9) Logout

------------------------------------------------------------
DATA FILES
------------------------------------------------------------
- data/accounts.csv
    Format: accountNumber,name,pinHash,balance

- data/transactions.csv
    Format: timestamp,accountNumber,type,amount,otherAccount,balanceAfter

------------------------------------------------------------
LICENSE
------------------------------------------------------------
MIT License
