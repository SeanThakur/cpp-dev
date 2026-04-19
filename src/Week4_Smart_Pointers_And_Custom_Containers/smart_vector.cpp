#include <iostream>
#include <memory>

class SmartVector
{
private:
    size_t size;
    size_t capacity;
    std::unique_ptr<int[]> data;

public:
    SmartVector()
    {
        std::cout << "---Constructor Called:--- \n";
        size = 0;
        capacity = 2;
        data = std::make_unique<int[]>(capacity);
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            size_t newCapacity = capacity * 2;
            std::cout << "---Reallocating the new capacity " << newCapacity << "\n";
            auto newData = std::make_unique<int[]>(newCapacity);
            for (size_t i = 0; i < size; i++)
            {
                newData[i] = std::move(data[i]);
            }
            data = std::move(newData);
            capacity = newCapacity;
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
    std::cout << "--Main program--\n";
    std::unique_ptr<SmartVector> vec = std::make_unique<SmartVector>();
    std::cout << "Vector size " << vec->getSize() << "\n";
    std::cout << "Vector capacity " << vec->getCapacity() << "\n";
    for (size_t i = 0; i < 5; i++)
    {
        vec->push_back(i);
    }
    return 0;
}
