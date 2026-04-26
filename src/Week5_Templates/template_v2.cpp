#include <iostream>

template <typename T>
class MyVector
{
public:
    MyVector()
    {
        std::cout << "[GENERIC] Created a standard vector.\n";
    }
};

// Template specialization for bool type
template <>
class MyVector<bool>
{
public:
    MyVector()
    {
        std::cout << "[WARNING] Created a heavily optimized boolean vector! Be careful!\n";
    }
};

int main()
{
    MyVector<int> IntVec;
    MyVector<double> doubleVec;
    MyVector<bool> boolVec;
    return 0;
}