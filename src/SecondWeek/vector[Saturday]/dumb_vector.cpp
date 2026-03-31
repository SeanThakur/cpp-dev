#include <iostream>

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
        std::cout << "Constructor called, allocated memory for vector with capacity: " << capacity << " \n";
    }

    ~Vector()
    {
        delete[] data;
        std::cout << "Desctructor called, deallocated memory for vector \n";
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            // Step A: Double the capacity
            capacity *= 2;
            std::cout << "Resizing vector, new capacity: " << capacity << " \n";
            int *newData = new int[capacity];

            // step B: copy the old data to the new array
            for (int i = 0; i < size; i++)
            {
                newData[i] = data[i];
            }
            std::cout << "Copied " << size << " elements to the new array \n";

            // Step C: Free the old memory and update the data pointer
            delete[] data;
            data = newData;
        }
        data[size] = value;
        size++;
        std::cout << "Added value: " << value << " to vector. Current size: " << size << " Capacity: " << capacity << "\n";
    }

    void print()
    {
        std::cout << "Vector contains: ";
        for (int i = 0; i < size; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }
};

int main()
{
    std::cout << "Starting Vector program \n\n";
    Vector myVector;
    myVector.push_back(10);
    myVector.push_back(20);
    myVector.push_back(30);
    myVector.push_back(40);
    myVector.print();
    std::cout << "\n Ending Vector program \n";
    return 0;
}