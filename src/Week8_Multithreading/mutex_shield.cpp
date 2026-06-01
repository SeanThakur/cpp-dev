#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

int main()
{
    std::cout << "--- WEDNESDAY: THE MUTEX SHIELD ---\n\n";
    int sharedCounter = 0;
    std::mutex counterMutex;
    std::vector<std::thread> poll;

    auto heavyLifting = [&sharedCounter, &counterMutex]()
    {
        for (int i = 0; i < 10000; i++)
        {
            std::lock_guard<std::mutex> lock(counterMutex);
            sharedCounter++;
        }
    };

    std::cout << "[Engine] Spawning 10 locked threads...\n";

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
    std::cout << "Actual Counter Value:   " << sharedCounter << " (Perfect!)\n";
    return 0;
}