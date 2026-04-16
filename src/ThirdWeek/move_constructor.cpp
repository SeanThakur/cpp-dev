#include <iostream>
#include <utility>

class HeapInt
{
public:
    int *ptr;

    HeapInt(int value)
    {
        ptr = new int(value);
        std::cout << "Constructor called with value: " << value << " At address " << ptr << "\n";
    }
    HeapInt(const HeapInt &source)
    {
        ptr = new int(*(source.ptr));
        std::cout << "Copy constructor called Deep copied the value " << *ptr << " To the new address " << ptr << "\n";
    }
    HeapInt(HeapInt &&source) noexcept
    {
        this->ptr = source.ptr; // steal the resource
        std::cout << "Move constructor called " << "Stealing the address " << ptr << "\n";
        source.ptr = nullptr; // null out the source pointer
    }
    ~HeapInt()
    {
        if (ptr != nullptr)
        {
            std::cout << "Destructor called for value: " << *ptr << "\n";
            delete ptr;
        }
        else
        {
            std::cout << "Destructor called for null pointer\n";
        }
    }
};

int main()
{
    HeapInt a(10);
    HeapInt b = a; // copy constructor
    HeapInt c(20);
    HeapInt d = std::move(c); // move constructor
    std::cout << "Value of d the move constructor that steal the c data: " << *d.ptr << " and the address " << d.ptr << "\n";
    return 0;
}