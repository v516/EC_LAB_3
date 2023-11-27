#include <iostream>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):\n"
                  << "   Holder: " << accountHolder << "\n"
                  << "   Balance: $" << balance << "\n";
    }

    double getBalance() const {
        return balance;
    }

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited $" << amount << " into the account.\n";
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the account.\n";
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance.\n";
        }
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << (interestRate * 100) << "%\n";
    }

    void withdraw(double amount) override {
        const double minBalance = 100;  // Minimum balance required for a savings account
        if (amount > 0 && (getBalance() - amount) >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the savings account.\n";
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance (minimum balance: $"
                      << minBalance << ").\n";
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << overdraftLimit << "\n";
    }

    void withdraw(double amount) override {
        if (amount > 0 && (getBalance() - amount) >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the current account.\n";
        } else {
            std::cout << "Invalid withdrawal amount or overdraft limit exceeded.\n";
        }
    }

    // Operator overloading for transferring money between accounts
    CurrentAccount& operator+(const SavingsAccount& other) {
        double transferAmount = 300;  // Adjust this as needed
        if (transferAmount <= other.getBalance()) {
            balance += transferAmount;
            std::cout << "Transferred $" << transferAmount << " from savings to current account.\n";
        } else {
            std::cout << "Transfer failed: Insufficient balance in the source account.\n";
        }
        return *this;
    }
};

// Operator overloading for displaying account details
std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << savings;
    std::cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    std::cout << savings;
    std::cout << current;

    return 0;
}
