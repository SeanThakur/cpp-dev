#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

std::mutex coutMutex;

std::atomic<int> health{100};

void modifyHealth(int amount, int threadId)
{
    int expected = health.load();
    int desired;

    do
    {
        desired = expected + amount;

        if (desired > 100)
            desired = 100;
        if (desired < 0)
            desired = 0;
    } while (!health.compare_exchange_weak(expected, desired));

    // std::cout << "Thread ID : " << threadId << " Amount: " << amount << " Desired: " << desired << " \n";
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "[Thread " << threadId << "] Applied " << amount
                  << ". New Health: " << desired << "\n";
    }
}

int main()
{
    std::cout << "--- TUESDAY: COMPARE-AND-SWAP ---\n\n";
    std::vector<std::thread> poll;

    for (int i = 1; i <= 10; i++)
    {
        poll.emplace_back(std::thread(modifyHealth, -15 + (i * 2), i));
    }

    for (auto &t : poll)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    return 0;
}