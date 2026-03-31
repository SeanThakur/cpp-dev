#include <iostream>

int *heapInspection()
{
    int *heapVar = new int;
    *heapVar = 99;
    std::cout << "Inside heapInspection function \n";
    std::cout << "heapVar address: " << (void *)heapVar << "\n";
    return heapVar;
}

int main()
{
    // Basic of heap
    std::cout << "Inside Main \n";
    int *heapPtr = heapInspection();
    std::cout << "Back in main \n";
    std::cout << "heapPtr address: " << heapPtr << "\n";
    delete heapPtr;
    heapPtr = nullptr;
    if (heapPtr == nullptr)
    {
        std::cout << "heapPtr is now null after deletion.\n";
    }
    // advance of heap
    std::cout << "Allocating memory to heap var \n";
    int *allocPtr = new int;
    *allocPtr = 42;
    std::cout << "First delete: Returning memory to the OS \n";
    delete allocPtr;
    std::cout << "Second delete: Attempting to delete already freed memory \n";
    delete allocPtr; // This is undefined behavior and can cause a crash or other issues
    std::cout << "After second delete: This may cause undefined behavior \n";
    return 0;
}