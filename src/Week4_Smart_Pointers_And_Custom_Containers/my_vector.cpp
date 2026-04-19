#include <iostream>

class MyVector
{
private:
    size_t size;
    size_t capacity;
    int *data;

public:
    MyVector()
    {
        std::cout << "----Constructor called---- \n";
        size = 0;
        capacity = 2;
        data = new int[capacity];
    }

    ~MyVector()
    {
        std::cout << "----Destructor called---- \n";
        delete[] data;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            size_t newCapacity = capacity * 2;
            std::cout << "--> Reallocating memory! New capacity: " << newCapacity << "\n";
            capacity = newCapacity;
            int *newData = new int[newCapacity];
            for (size_t i = 0; i < size; i++)
            {
                newData[i] = std::move(data[i]);
            }
            delete[] data;
            data = newData;
        }

        data[size] = value;
        size++;
    }

    size_t getSize()
    {
        return size;
    }

    size_t getCapacity()
    {
        return capacity;
    }
};

int main()
{
    std::cout << "---Main program---- \n";
    MyVector vec = MyVector();
    std::cout << "Vector initial size " << vec.getSize() << "\n";
    std::cout << "Vector initial capacity " << vec.getCapacity() << "\n";
    for (size_t i = 0; i < 5; i++)
    {
        vec.push_back(i);
    }
    std::cout << "---End of the main program---- \n";
    return 0;
}