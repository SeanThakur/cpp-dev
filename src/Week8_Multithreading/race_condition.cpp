#include <iostream>
#include <vector>
#include <thread>

int main()
{
    std::cout << "--- TUESDAY: THE RACE CONDITION ---\n\n";
    std::vector<std::thread> poll;
    int shared_counter = 0;
    auto heavyLifting = [&shared_counter]()
    {
        for (int i = 0; i < 10000; i++)
        {
            shared_counter++;
        }
    };

    std::cout << "[Engine] Spawning 10 threads to process 10,000 operations each...\n";

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
    std::cout << "Actual Counter Value:   " << shared_counter << "\n";
    return 0;
}