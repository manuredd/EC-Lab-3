#include <iostream>
#include <iomanip>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    // Constructor
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    // Methods
    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):\n"
                  << "   Holder: " << accountHolder << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposit successful. New balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
        } else {
            std::cout << "Invalid deposit amount. Please enter a positive value.\n";
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful. New balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance.\n";
        }
    }

    // Getters for protected members
    const std::string& getAccountNumber() const {
        return accountNumber;
    }

    const std::string& getAccountHolder() const {
        return accountHolder;
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    // Constructor
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    // Override the withdraw method
    void withdraw(double amount) override {
        const double minBalance = 100.0;
        if (amount > 0 && balance - amount >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawal successful. New balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance (must maintain a minimum balance of $100).\n";
        }
    }

    void displayDetails() const override {
        std::cout << "Account Details for Savings Account (ID: " << getAccountNumber() << "):\n"
                  << "   Holder: " << getAccountHolder() << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << getBalance() << "\n"
                  << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%\n";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    // Constructor
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double overdraft)
        : Account(number, holder, initialBalance), overdraftLimit(overdraft) {}

    // Override the withdraw method
    void withdraw(double amount) override {
        if (amount > 0 && getBalance() + overdraftLimit >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful. New balance: $" << std::fixed << std::setprecision(2) << getBalance() << "\n";
        } else {
            std::cout << "Invalid withdrawal amount or overdraft limit exceeded.\n";
        }
    }

    void displayDetails() const override {
        std::cout << "Account Details for Current Account (ID: " << getAccountNumber() << "):\n"
                  << "   Holder: " << getAccountHolder() << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << getBalance() << "\n"
                  << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << "\n";
    }
};

// Overload the + operator to allow transferring money between accounts
CurrentAccount operator+(CurrentAccount& current, SavingsAccount& savings) {
    double transferAmount = 300.0; // You can modify the transfer amount as needed
    if (savings.getBalance() >= transferAmount) {
        current.deposit(transferAmount);
        savings.withdraw(transferAmount);
    } else {
        std::cout << "Insufficient balance in the savings account for the transfer.\n";
    }
    return current;
}

// Overload the << operator to display account details
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
