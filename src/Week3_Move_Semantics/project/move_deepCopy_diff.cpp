#include <iostream>
#include <chrono>
#include <utility>

class HeapArray
{
private:
    size_t size;
    int *data;

public:
    HeapArray(size_t value)
    {
        size = value;
        data = new int[size];
        // Initialize the array with some values
        for (int i = 0; i < size; i++)
        {
            data[i] = i;
        }
    }
    ~HeapArray()
    {
        if (data != nullptr)
        {
            delete[] data;
        }
    }
    // Deep copy constructor
    HeapArray(const HeapArray &source)
    {
        size = source.size;
        data = new int[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = source.data[i];
        }
    }
    // move constructor
    HeapArray(HeapArray &&source) noexcept
    {
        this->size = source.size;
        this->data = source.data; // steal the resource
        source.data = nullptr;    // null out the source pointer
        source.size = 0;          // reset the source size
    }
};

int main()
{
    std::cout << "--Performance Benchmark--\n";
    const int ARRAY_SIZE = 1000000;
    const int NUM_ITERATIONS = 100;

    std::cout << "Phase 1: Deep Copy Constructor\n";
    HeapArray origalArray(ARRAY_SIZE);
    std::vector<HeapArray> copyArray; // Deep copy constructor
    copyArray.reserve(NUM_ITERATIONS);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        copyArray.push_back(origalArray);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> copyDuration = end - start;
    std::cout << "Time taken for deep copy constructor: " << copyDuration.count() << " seconds\n";

    std::cout << "\nPhase 2: Move Constructor\n";
    std::vector<HeapArray> tempData; // Move constructor
    tempData.reserve(NUM_ITERATIONS);
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        tempData.push_back(HeapArray(ARRAY_SIZE)); // create temporary objects
    }

    std::vector<HeapArray> moveArray; // Move constructor
    moveArray.reserve(NUM_ITERATIONS);
    auto moveStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        moveArray.push_back(std::move(tempData[i])); // move the temporary objects
    }
    auto moveEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> moveDuration = moveEnd - moveStart;
    std::cout << "Time taken for move constructor: " << moveDuration.count() << " seconds\n";

    return 0;
}