#include <iostream>
#include <memory>

// void processData(std::unique_ptr<int[]> &data)
// This is a reference to a unique_ptr, which allows the function to access the unique_ptr without
// taking ownership of it. The caller retains ownership of the unique_ptr, and the function
// can read or modify the data as needed. However, the function cannot transfer ownership or
// move the unique_ptr to another variable. This is useful when you want to pass a unique_ptr to a
// function for processing without giving up ownership.
void processData(std::unique_ptr<int[]> data)
{
    std::cout << "--> Processing data..." << std::endl;
    std::cout << "--> Processing data is here " << data[0] << std::endl;
}

void simulateHeavyWork()
{
    std::cout << "--> Simulating heavy work..." << std::endl;
    std::unique_ptr<int[]> massiveArray = std::make_unique<int[]>(1000000);
    massiveArray[0] = 99;
    // processData(massiveArray);
    // This will cause a compile-time error because processData expects a unique_ptr,
    // but we are passing a reference to it. To fix this, we need to move the unique_ptr to transfer ownership to the processData function.
    // massiveArray[0] = 50; // Perfectly safe. You still own it.
    processData(std::move(massiveArray)); // This will transfer ownership of the unique_ptr to the processData function.
    // After moving, massiveArray is now empty (nullptr), and we cannot access it anymore.
    // massiveArray[0] = 50; // CRASH! You just moved the memory away. massiveArray is now a hollow shell (nullptr).
    std::cout << "--> simulateHeavyWork() is ending. The invisible bulldozer is waiting...\n";
}

int main()
{
    std::cout << "Program is starting...\n";
    simulateHeavyWork();
    std::cout << "Program is ending...\n";
    return 0;
}