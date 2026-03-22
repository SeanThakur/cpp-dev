#include <iostream>
#include <unistd.h>

int main()
{
    std::cout << "This is memory leak simulator. \n";
    std::cout << "Open your new terminal and run the command: top -o MEM \n";
    std::cout << "Press enter to start the memory leak simulation. \n";
    std::cin.get();

    long long leakedMemoryMb = 0;
    while (true)
    {
        // Allocating 10 MB of memory
        int *massiveMemory = new int[2500000];
        for (int i = 0; i < 2500000; i += 1000)
        {
            massiveMemory[i] = 1; // Just to use the allocated memory
        }

        leakedMemoryMb += 10;
        std::cout << "Leaked Memory: " << leakedMemoryMb << " MB of physical RAM.... \n";
        // Delaying the next allocation by 50 milliseconds to make it easier to observe the memory leak in the system monitor
        usleep(50000);
    }

    return 0;
}