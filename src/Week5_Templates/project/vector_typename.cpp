#include <iostream>
#include <string>
#include <utility>

template <typename T>
class MyVector
{
private:
    int capacity;
    int currentSize;
    T *data;

public:
    MyVector(int cap = 5)
    {
        capacity = cap;
        currentSize = 0;
        data = new T[capacity];
    }

    ~MyVector()
    {
        delete[] data;
    }

    MyVector(const MyVector &other)
    {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];

        for (int i = 0; i < currentSize; i++)
        {
            data[i] = other.data[i];
        }
    }

    MyVector(MyVector &&other) noexcept
    {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = std::move(other.data);

        other.data = nullptr;
        other.capacity = 5;
        other.currentSize = 0;
    }

    MyVector &operator=(const MyVector &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (data != nullptr)
        {
            delete[] data;
        }

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    MyVector &operator=(MyVector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (data != nullptr)
        {
            delete[] data;
        }

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = std::move(other.data);

        other.data = nullptr;
        other.currentSize = 0;
        other.capacity = 5;

        return *this;
    }

    void push(T element)
    {
        if (currentSize < capacity)
        {
            data[currentSize] = element;
            currentSize++;
        }
        else
        {
            std::cout << "[!] Vector is full! (Resizing logic goes here)\n";
        }
    }

    void print()
    {
        std::cout << "[ ";
        for (int i = 0; i < currentSize; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << "]\n";
    }
};

int main()
{
    std::cout << "Vector in INT \n";
    MyVector<int> vecInt;
    vecInt.push(10);
    vecInt.push(20);
    vecInt.push(30);
    vecInt.push(40);
    vecInt.print();
    std::cout << "1. Triggering Copy Constructor:\n";
    MyVector<int> copyVec = vecInt;

    std::cout << "\n2. Triggering Copy Assignment:\n";
    MyVector<int> assignedVec;
    assignedVec = vecInt;
    std::cout << "Vector in String \n";
    MyVector<std::string> vecString;
    vecString.push("SEAN");
    vecString.push("VIKAS");
    vecString.push("RAHUL");
    vecString.push("PAUL");
    vecString.print();

    MyVector<std::string> arsenal;
    arsenal.push("Plasma Rifle");
    arsenal.push("Rocket Launcher");

    std::cout << "--- BEFORE THE THEFT ---\n";
    arsenal.print();

    // Trigger the Move Constructor!
    std::cout << "\n--- EXECUTING HEIST ---\n";
    MyVector<std::string> stolenArsenal = std::move(arsenal);

    std::cout << "\n--- AFTER THE THEFT ---\n";
    std::cout << "Stolen Arsenal: ";
    stolenArsenal.print();

    std::cout << "Original Arsenal (Should be empty): ";
    arsenal.print(); // This will print [] because currentSize is now 0!
    return 0;
}