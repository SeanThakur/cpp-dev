#include <iostream>
#include <memory>
#include <utility>

class MySmartVector
{
private:
    size_t size;
    size_t capacity;
    std::unique_ptr<int[]> data;

public:
    MySmartVector()
    {
        std::cout << "---Constructor---\n";
        size = 0;
        capacity = 2;
        data = std::make_unique<int[]>(capacity);
    }

    MySmartVector(const MySmartVector &source)
    {
        std::cout << "---Copy Constructor---\n";
        size = source.size;
        capacity = source.capacity;
        data = std::make_unique<int[]>(capacity);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
    }

    MySmartVector &operator=(const MySmartVector &source)
    {
        std::cout << "---Copy Assignment Constructor---\n";
        if (this == &source)
        {
            return *this;
        }
        size = source.size;
        capacity = source.capacity;
        data = std::make_unique<int[]>(capacity);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
        return *this;
    }

    MySmartVector(MySmartVector &&source) noexcept
    {
        std::cout << "---Move Constructor---\n";
        this->size = source.size;
        this->capacity = source.capacity;
        this->data = std::move(source.data);

        source.size = 0;
        source.capacity = 0;
    }

    MySmartVector &operator=(MySmartVector &&source) noexcept
    {
        std::cout << "---Move Assignment Constructor---\n";
        if (this == &source)
        {
            return *this;
        }

        this->size = source.size;
        this->capacity = source.capacity;
        this->data = std::move(source.data);

        source.size = 0;
        source.capacity = 0;

        return *this;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            size_t newCapacity = capacity * 2;
            std::cout << "Reallocating new capacity " << newCapacity << "\n";
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

// int main()
// {
//     std::unique_ptr<MySmartVector> vec1 = std::make_unique<MySmartVector>();
//     std::cout << "Vector size " << vec1->getSize() << "\n";
//     std::cout << "Vector capacity " << vec1->getCapacity() << "\n";
//     for (size_t i = 0; i < 5; i++)
//     {
//         vec1->push_back(i);
//     }

//     return 0;
// }

int main()
{
    std::cout << "--- THE SMART VECTOR BENCHMARK ---\n";

    MySmartVector v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    v1.push_back(60);
    v1.push_back(70);

    std::cout << "\n1. Testing Copy Constructor...\n";
    MySmartVector v2 = v1; // Deep copies the data into a new unique_ptr

    std::cout << "\n2. Testing Copy Assignment...\n";
    MySmartVector v3;
    v3 = v1; // Deep copies the data into v3's unique_ptr

    std::cout << "\n3. Testing Move Constructor...\n";
    MySmartVector v4 = std::move(v1); // Steals the unique_ptr out of v1

    std::cout << "\n4. Testing Move Assignment...\n";
    MySmartVector v5;
    v5 = std::move(v2); // Steals the unique_ptr out of v2

    std::cout << "\nBenchmark complete. Watch it clean up perfectly without a single Destructor log.\n";
    return 0;
}