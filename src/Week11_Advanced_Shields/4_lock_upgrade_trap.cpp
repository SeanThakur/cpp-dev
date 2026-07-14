#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>

std::shared_mutex rwMutex;
int playerGold = 100;

void attemptPurchase(int threadId)
{
    std::cout << "[Thread " << threadId << "] Checking inventory...\n";
    {
        std::shared_lock<std::shared_mutex> lock(rwMutex);
        if (playerGold < 20)
        {
            std::cout << "[Thread " << threadId << "] Not enough gold. Leaving.\n";
            return;
        }
        else
        {
            std::cout << "[Thread " << threadId << "] Gold looks good. Preparing to buy...\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::unique_lock<std::shared_mutex> lock(rwMutex);
    if (playerGold >= 20)
    {
        playerGold -= 20;
        std::cout << "  -> [Thread " << threadId << "] SUCCESS! Bought the sword. Gold left: " << playerGold << "\n";
    }
    else
    {
        std::cout << "  -> [Thread " << threadId << "] FAILED! Another thread took the gold while I was upgrading my lock!\n";
    }
}

int main()
{
    std::cout << "--- THURSDAY PART 2: THE LOCK UPGRADE TRAP ---\n\n";
    std::vector<std::thread> worker;
    for (int i = 1; i <= 7; i++)
    {
        worker.push_back(std::thread(attemptPurchase, i));
    }
    for (auto &t : worker)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    return 0;
}