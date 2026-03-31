#include <iostream>

void hackTheAccount(int *accountBalance)
{
    if (accountBalance == nullptr)
    {
        std::cout << "Error: pointer is null.\n";
        return;
    }
    *accountBalance *= 2;
    std::cout << "Account balance is now hacked and doubled remotely: \n";
}

int main()
{
    // basic pointer example
    int powerLevel = 9000;
    int *powerLevelPtr = &powerLevel;
    std::cout << powerLevel << "\n";
    std::cout << "Power level address: " << &powerLevel << "\n";
    std::cout << "Power level pointer: " << powerLevelPtr << "\n";
    std::cout << "Power level value via pointer: " << *powerLevelPtr << "\n";
    *powerLevelPtr = 9001;
    std::cout << "Power level after update: " << powerLevel << "\n";
    // advance pointer example
    int accountBalance = 500;
    std::cout << "Account balance before hack: " << accountBalance << "\n";
    hackTheAccount(&accountBalance);
    std::cout << "Account balance after hack: " << accountBalance << "\n";
    int *safeptr = nullptr;
    std::cout << "Attempting to hack a null pointer...\n";
    hackTheAccount(safeptr);
    std::cout << "size of account balance: " << sizeof(accountBalance) << "\n";
    std::cout << "size of pointer: " << sizeof(&accountBalance) << "\n";
    return 0;
}
