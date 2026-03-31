#include <iostream>
#include <cstdlib>

struct MemoryMetrics
{
    size_t totalAllocated = 0;
    size_t totalFreed = 0;

    size_t currentMemorySize()
    {
        return totalAllocated - totalFreed;
    }
};

MemoryMetrics _mMetrics;

void *operator new(size_t size)
{
    std::cout << "Allocating " << size << " bytes of memory. \n";
    _mMetrics.totalAllocated += size;
    return malloc(size);
}

void operator delete(void *node, size_t size) noexcept
{
    std::cout << "Deallocating " << size << " bytes of memory. \n";
    _mMetrics.totalFreed += size;
    free(node);
}

int main()
{
    std::cout << "Memory Tracker Simulator. \n";
    std::cout << "Inital Memory Usage " << _mMetrics.currentMemorySize() << " bytes. \n";

    std::cout << "Creating a single memory integer. \n";
    int *safePtr = new int;
    *safePtr = 42;

    std::cout << "Creating a array of integers. \n";
    int *leakedArray = new int[1000];

    std::cout << "[Mid Simulation] Current Memory Usage: " << _mMetrics.currentMemorySize() << " bytes. \n";
    std::cout << "Deleting the single memory integer. \n";
    delete safePtr;

    std::cout << "Fnal Memory Usage: \n";
    std::cout << "Total Allocated: " << _mMetrics.totalAllocated << " bytes. \n";
    std::cout << "Total Freed: " << _mMetrics.totalFreed << " bytes. \n";

    size_t finalLeakedMemory = _mMetrics.currentMemorySize();

    if (finalLeakedMemory > 0)
    {
        std::cout << "Memory Leak Detected: " << finalLeakedMemory << " bytes of memory is still allocated and not freed. \n";
    }
    else
    {
        std::cout << "No Memory Leak Detected: All allocated memory has been freed. \n";
    }

    return 0;
}