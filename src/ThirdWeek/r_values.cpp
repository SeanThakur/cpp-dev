#include <iostream>
#include <utility>

void printType(int &x)
{
    std::cout << "LValue: " << x << "\n";
    std::cout << "Address: " << &x << "\n";
}

void printType(int &&x)
{
    std::cout << "RValue: " << x << "\n";
    std::cout << "Address: " << &x << "\n";
}

void routerOverrideType(int &x)
{
    std::cout << "LValue: Cought Permanent LValue: " << x << "\n";
}

void routerOverrideType(int &&x)
{
    std::cout << "RValue: Cought Temporary RValue: " << x << "\n";
    std::cout << "Inside R Value, passing 'x' forward...\n";
    routerOverrideType(x); // This will call the LValue version because 'x' is
}

int main()
{
    std::cout << "TEST 1: Passing an LValue:\n";
    int a = 10;
    printType(a); // LValue

    std::cout << "\nTEST 2: Passing an RValue:\n";
    printType(10 + 10); // RValue

    std::cout << "\nTEST 3: added raw literal number:\n";
    printType(42); // RValue

    std::cout << "\nTEST 4: passing an LValue to routerOverrideType:\n";
    routerOverrideType(a);

    std::cout << "\nTEST 5: passing an RValue to routerOverrideType using std::move:\n";
    routerOverrideType(std::move(a)); // std::move casts 'a' to an RValue reference

    return 0;
}