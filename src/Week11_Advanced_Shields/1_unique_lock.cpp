#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex configMutex;
int sharedData = 100;

void updateAndProcessScore(int threadId)
{
    std::unique_lock<std::mutex> lock(configMutex);
    sharedData += 10;
    std::cout << "[Thread " << threadId << "] Incremented score to: " << sharedData << "\n";
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    lock.lock();
    std::cout << "[Thread " << threadId << "] Post-processing finished safely.\n";
}

int main()
{
    std::cout << "--- WEEK 11 MONDAY: THE FLEXIBLE SHIELD ---\n\n";
    std::vector<std::thread> poll;

    for (int i = 0; i < 3; i++)
    {
        poll.push_back(std::thread(updateAndProcessScore, i));
    }

    for (auto &w : poll)
    {
        if (w.joinable())
        {
            w.join();
        }
    }
    std::cout << "\n[Main] Final Score: " << sharedData << "\n";
    return 0;
}