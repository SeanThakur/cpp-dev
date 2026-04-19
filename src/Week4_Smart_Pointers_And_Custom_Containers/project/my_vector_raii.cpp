#include <iostream>
#include <utility>

class MyVector
{
private:
    size_t size;
    size_t capacity;
    int *data;

public:
    MyVector()
    {
        std::cout << "----Constructor----\n";
        size = 0;
        capacity = 2;
        data = new int[capacity];
    }

    MyVector(const MyVector &source)
    {
        std::cout << "----Copy Constructor----\n";
        size = source.size;
        capacity = source.capacity;
        data = new int[capacity];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
    }

    MyVector &operator=(const MyVector &source)
    {
        std::cout << "----Copy Assignment Constructor----\n";
        if (this == &source)
        {
            return *this;
        }
        if (data != nullptr)
        {
            delete[] data;
        }
        size = source.size;
        capacity = source.capacity;
        data = new int[capacity];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
        return *this;
    }

    MyVector(MyVector &&source) noexcept
    {
        std::cout << "----Move Constructor----\n";
        this->size = source.size;
        this->capacity = source.capacity;
        this->data = source.data;

        source.data = nullptr;
        source.size = 0;
        source.capacity = 0;
    }

    MyVector &operator=(MyVector &&source) noexcept
    {
        std::cout << "----Move Assignment Constructor----\n";
        if (this == &source)
        {
            return *this;
        }
        if (data != nullptr)
        {
            delete[] data;
        }

        this->size = source.size;
        this->capacity = source.capacity;
        this->data = source.data;

        source.data = nullptr;
        source.size = 0;
        source.capacity = 0;
        return *this;
    }

    ~MyVector()
    {
        std::cout << "----Desctructor----\n";
        delete[] data;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            size_t newCapacity = capacity * 2;
            std::cout << "----Reallocation of new capacit----" << newCapacity << "\n";
            int *newData = new int[newCapacity];
            for (size_t i = 0; i < size; i++)
            {
                newData[i] = std::move(data[i]);
            }
            delete[] data;
            data = newData;
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
    std::cout << "----Main Programm----\n";
    MyVector v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    std::cout << "\n1. Testing Copy Constructor...\n";
    MyVector v2 = v1;

    std::cout << "2. Testing Copy Assignment...\n";
    MyVector v3;
    v3 = v1;

    std::cout << "3. Testing Move Constructor...\n";
    MyVector v4 = std::move(v1);

    std::cout << "4. Testing Move Assignment...\n";
    MyVector v5;
    v5 = std::move(v2);

    std::cout << "\nIf this prints, and the program ends without a Double Free crash, you are a master of raw C++ memory.\n";
    std::cout << "----End of Main Programm----\n";
    return 0;
}
