#include <iostream>

int main()
{
    std::cout << "Hello, World! \n";
    int x = 9;
    int y = 10;
    int &ref = x;
    std::cout << "x: " << x << "\n";
    std::cout << "y: " << y << "\n";
    std::cout << "ref: " << ref << "\n";
    ref = y;
    std::cout << "[After] x: " << x << "\n";
    std::cout << "[After] y: " << y << "\n";
    std::cout << "[After] ref: " << ref << "\n";
    std::cout << "End of practise. \n";
    std::cout << "Address of ref " << &ref << " and y " << &y << "\n";
    return 0;
}