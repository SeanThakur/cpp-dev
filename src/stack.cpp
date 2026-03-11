#include <iostream>

int *pointAddress()
{
    int localVar = 99;
    return &localVar;
}

void inspectStackFrame(int param1, int param2)
{
    int localVar1 = 11;
    int localVar2 = 12;
    std::cout << "inside the inspectStackFrame function \n";
    std::cout << "param 1: " << (void *)&param1 << "\n";
    std::cout << "param 2: " << (void *)&param2 << "\n";
    std::cout << "localVar 1: " << (void *)&localVar1 << "\n";
    std::cout << "localVar 2: " << (void *)&localVar2 << "\n";
}

int main()
{
    int mainVar = 12;
    std::cout << "inside the main function \n";
    std::cout << "mainVar: " << (void *)&mainVar << "\n";
    inspectStackFrame(1, 2);
    std::cout << "END of inspectStackFrame \n";
    int *ptr = pointAddress();
    std::cout << "Address of secret variable: " << (void *)ptr << "\n";
    return 0;
}