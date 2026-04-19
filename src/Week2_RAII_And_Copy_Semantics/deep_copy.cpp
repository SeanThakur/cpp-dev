#include <iostream>

class HeapInt
{
private:
    int *ptr;

public:
    HeapInt(int initialValue)
    {
        ptr = new int;
        *ptr = initialValue;
        std::cout << "Constructor, allocated memory at " << (void *)ptr << " with value: " << initialValue << "\n";
    }

    // Deep copy constructor
    HeapInt(const HeapInt &source)
    {
        ptr = new int;        // Allocate new memory
        *ptr = *(source.ptr); // Copy the value from the source object
        std::cout << "Deep Copy: Allocated NEW memory at " << (void *)ptr << " with value: " << *ptr << "\n";
    }

    ~HeapInt()
    {
        std::cout << "Destructor, deallocated memory at " << (void *)ptr << "\n";
        delete ptr;
        std::cout << "Successfully freed\n";
    }

    int getValue()
    {
        return *ptr;
    }
};

int main()
{
    std::cout << "Starting Deep copy program \n\n";
    HeapInt objA(99);
    std::cout << "Copying objA to objB \n";
    HeapInt objB = objA; // This will call the deep copy constructor
    std::cout << "\n Obj A value: " << objA.getValue() << "\n";
    std::cout << "\n Obj B value: " << objB.getValue() << "\n";
    std::cout << "\n Ending Deep copy program \n";
    return 0;
}