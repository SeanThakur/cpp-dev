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

    // descructor
    ~HeapInt()
    {
        delete ptr;
        std::cout << "Destructor, deallocated memory at " << (void *)ptr << "\n";
    }

    // copy constructor
    HeapInt(const HeapInt &source)
    {
        ptr = new int;
        *ptr = *(source.ptr);
        std::cout << "Copy Constructor, allocated memory at " << (void *)ptr << " with value: " << *ptr << "\n";
    }

    // copy assignment operator
    HeapInt &operator=(const HeapInt &source)
    {
        if (this == &source)
        {
            std::cout << "Self-assignment detected, no action taken \n";
            return *this;
        }
        std::cout << "Bulldozing existing resource at " << (void *)ptr << " to allocate new memory \n";
        delete ptr;           // Free existing resource
        ptr = new int;        // Allocate new memory
        *ptr = *(source.ptr); // Copy the value from the source object
        std::cout << "Copy Assignment Operator, allocated memory at " << (void *)ptr << " with value: " << *ptr << "\n";

        return *this;
    }
};

int main()
{
    std::cout << "Starting Rule of Three program \n\n";
    HeapInt objA(99);
    HeapInt objB(50);
    HeapInt objC(25);

    std::cout << "Copying objA to objB using copy constructor \n";
    objB = objA; // This will call the copy assignment operator
    std::cout << "Testing self assignment on objC \n";
    objC = objC; // This will test self-assignment
    std::cout << "\n Ending Rule of Three program \n";
    return 0;
}