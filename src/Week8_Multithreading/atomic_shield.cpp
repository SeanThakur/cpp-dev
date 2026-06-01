#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

int main()
{
    std::cout << "--- WEDNESDAY PART 2: LOCK-FREE ATOMICS ---\n\n";
    // atomic we can use instead of mutex for premitives data types like (int, bool, or a pointer) it's very fast compare to mutex
    // but for complex data structure (like adding elements to a std::vector, writing to a File, or updating a Player class), you must use std::mutex. Atomics cannot protect complex RAM layouts.
    std::atomic<int> counter{0};
    std::vector<std::thread> poll;

    auto heavyLifting = [&counter]()
    {
        for (int i = 0; i < 10000; i++)
        {
            counter++;
        }
    };

    std::cout << "[Engine] Spawning 10 ATOMIC threads...\n";

    for (int i = 0; i < 10; i++)
    {
        poll.push_back(std::thread(heavyLifting));
    }

    for (auto &worker : poll)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }

    std::cout << "\n[Engine] All threads joined.\n";
    std::cout << "Expected Counter Value: 100000\n";
    std::cout << "Actual Counter Value:   " << counter << " (Perfect AND Fast!)\n";
    return 0;
}