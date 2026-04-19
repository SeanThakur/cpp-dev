#include <iostream>
#include <stdexcept>

class HeapInt
{
private:
    int *ptr;

public:
    HeapInt(int initalValue)
    {
        ptr = new int;
        *ptr = initalValue;
        std::cout << "Constructor called, allocated heap memory for int with value: " << initalValue << " \n";
    }

    ~HeapInt()
    {
        delete ptr;
        std::cout << "Destructor called, deallocating heap memory \n";
    }

    int getValue()
    {
        return *ptr;
    }
};

void simulateScope()
{
    std::cout << "Entering scope \n";
    HeapInt myInt(99);
    std::cout << "Reading value from HeapInt: " << myInt.getValue() << " \n";
    std::cout << "\n FATAL ERROR: Simulating an exception \n";
    throw std::runtime_error("Database Exception Found!! Connection Lost!!");
    std::cout << "Exiting scope \n";
}

int main()
{
    std::cout << "Starting Main program \n\n";
    try
    {
        simulateScope();
    }
    catch (const std::exception &e)
    {
        std::cout << "Cought an exception in main " << e.what() << " \n";
    }
    std::cout << "\n Ending Main program \n";
    return 0;
}