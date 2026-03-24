#include <iostream>

class HeapInt
{
private:
    int *safePtr;

public:
    HeapInt(int data)
    {
        safePtr = new int;
        *safePtr = data;
        std::cout << "Constructer called, allocated Heap memory for int with value: " << data << " \n";
    }

    ~HeapInt()
    {
        delete safePtr;
        std::cout << "Destructor called, deallocating Heap memory \n";
    }

    int getValue()
    {
        return *safePtr;
    }
};

void simulateScope()
{
    std::cout << "Entering scope \n";
    HeapInt myInt(99);
    std::cout << "Reading value from HeapInt: " << myInt.getValue() << " \n";
    std::cout << "Exiting scope \n";
}

int main()
{
    std::cout << "Starting program \n\n";
    simulateScope();
    std::cout << "\nProgram ended \n";
    std::cout << "Note: we have not used delete anywhere in our main function, yet the destructor is called automatically when the object goes out of scope, ensuring proper resource management. \n";
    return 0;
}