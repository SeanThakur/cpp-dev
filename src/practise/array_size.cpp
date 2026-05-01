#include <iostream>

template <typename T, size_t N>
constexpr size_t array_size(T (&)[N]) noexcept
{
    return N;
}

int main()
{
    const char *myArray[] = {"Hello", "World", "This", "Is", "A", "Test", "Array", "With", "10", "Elements"};
    std::cout << "Size of myArray: " << array_size(myArray) << std::endl;

    double anotherArray[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << "Size of anotherArray: " << array_size(anotherArray) << std::endl;

    return 0;
}