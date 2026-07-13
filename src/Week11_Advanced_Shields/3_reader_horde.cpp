#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <chrono>

std::shared_mutex shMutex;
int sharedConfigValue = 10;

void readConfigWorker(int threadId)
{
    std::shared_lock<std::shared_mutex> lock(shMutex);
    std::cout << "[Thread " << threadId << "] Reading config value: "
              << sharedConfigValue << " at time "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                         .count() %
                     100000
              << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main()
{
    std::cout << "--- WEDNESDAY: THE READER HORDE ---\n\n";
    std::vector<std::thread> worker;
    for (int i = 0; i <= 10; i++)
    {
        worker.push_back(std::thread(readConfigWorker, i));
    }

    for (auto &w : worker)
    {
        if (w.joinable())
        {
            w.join();
        }
    }
    return 0;
}