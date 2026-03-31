#include <iostream>
#include <cstdlib>
#include <stdexcept>

struct Metrics
{
    size_t totalAllocated = 0;
    size_t totalFreed = 0;
    size_t currentMemorySize()
    {
        return totalAllocated - totalFreed;
    }
};

Metrics _mMetrics;

void *operator new[](size_t size)
{
    std::cout << "Allocating " << size << " bytes of memory for array. \n";
    _mMetrics.totalAllocated += size;
    return malloc(size);
}

void operator delete[](void *node, size_t size) noexcept
{
    std::cout << "Deallocating " << size << " bytes of memory for array. \n";
    _mMetrics.totalFreed += size;
    return free(node);
}

class Vector
{
private:
    int size;
    int capacity;
    int *data;

public:
    Vector(int initalValue = 2)
    {
        size = 0;
        capacity = initalValue;
        data = new int[capacity];
        std::cout << "Constructor called, allocated memory for vector with capacity: " << capacity << std::endl;
    }

    // copy constructor
    Vector(const Vector &source)
    {
        size = source.size;
        capacity = source.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
    }

    // copy assignment operator
    Vector &operator=(const Vector &source)
    {
        if (this == &source)
        {
            return *this; // self assignment check
        }

        delete[] data; // free existing memory
        capacity = source.capacity;
        size = source.size;
        data = new int[capacity];
        // deep copy new data
        for (int i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }

        return *this;
    }

    ~Vector()
    {
        delete[] data;
        std::cout << "Destructor called, deallocated memory for vector" << std::endl;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            capacity *= 2;
            int *newData = new int[capacity];
            for (int i = 0; i < size; i++)
            {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size] = value;
        size++;
    }

    void print()
    {
        std::cout << "Vector contains: ";
        for (int i = 0; i < size; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    // operator[] overload for element access
    int &operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
};

int main()
{
    std::cout << "SECURE VECTOR Main Programm ===\n\n";
    Vector vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.print();
    Vector vec2 = vec; // copy constructor
    vec2.print();
    Vector vec3;
    vec3 = vec; // copy assignment operator
    vec3.print();

    std::cout << "Total allocated memory: " << _mMetrics.totalAllocated << " bytes\n";
    std::cout << "Total freed memory: " << _mMetrics.totalFreed << " bytes\n";
    std::cout << "Current memory usage: " << _mMetrics.currentMemorySize() << " bytes\n";

    if (_mMetrics.currentMemorySize() == 0)
    {
        std::cout << "No memory leak detected: All allocated memory has been freed.\n";
    }
    else
    {
        std::cout << "Memory leak detected: " << _mMetrics.currentMemorySize() << " bytes of memory is still allocated and not freed.\n";
    }

    return 0;
}