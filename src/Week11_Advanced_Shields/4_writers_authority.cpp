#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <vector>

std::shared_mutex rwMutex;
int sharedGameConfig = 100;

void readerWorker(int threadId)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(threadId * 10));
    std::cout << "[Reader " << threadId << "] Arrived at the door...\n";
    std::shared_lock<std::shared_mutex> lock(rwMutex);
    std::cout << "  -> [Reader " << threadId << "] Entered! Reading value: " << sharedGameConfig << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "[Reader " << threadId << "] Leaving.\n";
}

void writerWorker()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "\n[WRITER] Arrived! Requesting EXCLUSIVE access...\n";
    std::unique_lock<std::shared_mutex> lock(rwMutex);
    std::cout << "\n=============================================\n";
    std::cout << "[WRITER] GRANTED! Room is empty. Modifying data...\n";
    sharedGameConfig += 50;
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    std::cout << "[WRITER] Modification complete. Opening the floodgates!\n";
    std::cout << "=============================================\n\n";
}

int main()
{
    std::cout << "--- THURSDAY: THE WRITER'S AUTHORITY ---\n\n";
    std::vector<std::thread> crowd;

    for (int i = 0; i <= 4; i++)
    {
        crowd.push_back(std::thread(readerWorker, i));
    }

    std::thread writer(writerWorker);

    for (int i = 5; i <= 10; i++)
    {
        crowd.push_back(std::thread(readerWorker, i));
    }

    for (auto &cr : crowd)
    {
        if (cr.joinable())
        {
            cr.join();
        }
    }
    writer.join();
    std::cout << "\n[Engine] Simulation complete. Final Config: " << sharedGameConfig << "\n";
    return 0;
}