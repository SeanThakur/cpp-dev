#include <iostream>

int main()
{
    int userSize;
    std::cout << "Add the size of the array: ";
    std::cin >> userSize;

    int *dynamicArray = new int[userSize];
    std::cout << "Allocated the size " << userSize * 4 << " bytes for the array. \n";

    for (int i = 0; i < userSize; i++)
    {
        dynamicArray[i] = i * 10;
    }

    std::cout << "Data of the array 1 is: " << dynamicArray[0] << " | Address of this data is: " << (void *)&dynamicArray[0] << "\n";
    std::cout << "Data of the array 2 is: " << dynamicArray[1] << " | Address of this data is: " << (void *)&dynamicArray[1] << "\n";
    std::cout << "index of this array : " << userSize - 1 << " | Data of this array is: " << dynamicArray[userSize - 1] << " | Address of this data is: " << (void *)&dynamicArray[userSize - 1] << "\n";
    std::cout << "***** The pointer Math ***** \n";
    std::cout << "The second data is: " << dynamicArray[2] << " | Address of this data is: " << (void *)&dynamicArray[2] << "\n";
    std::cout << "The second data by using pointer match: " << *(dynamicArray + 2) << " | Address " << (void *)(dynamicArray + 2) << "\n";

    delete[] dynamicArray;
    dynamicArray = nullptr;

    std::cout << "Memory for the array has been deallocated. \n";
    return 0;
}