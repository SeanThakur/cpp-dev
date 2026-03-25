#include <iostream>

class HeapInt
{
private:
    int *ptr;

public:
    HeapInt(int initalValue)
    {
        ptr = new int;
        *ptr = initalValue;
        std::cout << "Constructor, allocated memory at " << (void *)ptr << " \n";
    }

    // Banning the copy constructor and assignment operator to prevent shallow copy
    // This will cause a compile-time error if we try to copy or assign HeapInt objects
    // HeapInt(const HeapInt &) = delete;
    // HeapInt &operator=(const HeapInt &) = delete;

    ~HeapInt()
    {
        std::cout << "Destructor, deallocated memory at " << (void *)ptr << " \n";
        delete ptr;
        std::cout << "Successfully freed" << " \n";
    }

    int getValue()
    {
        return *ptr;
    }
};

int main()
{
    std::cout << "Starting Shallow copy program \n\n";
    HeapInt objA(99);

    std::cout << "Copying objA to objB \n";
    HeapInt objB = objA;

    std::cout << "\n Obj A value: " << objA.getValue() << "\n";
    std::cout << "\n Obj B value: " << objB.getValue() << "\n";

    std::cout << "\n Ending Shallow copy program \n";
    return 0;
}