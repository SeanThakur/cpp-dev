#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

class ThreadPool
{
private:
    std::vector<std::thread> worker;
    static void workerLoop(int workerID)
    {
        std::cout << "[Worker " << workerID << "] Online and standing by.\n";
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

public:
    ThreadPool()
    {
        unsigned int coreCount = std::thread::hardware_concurrency();
        if (coreCount == 0)
            coreCount = 4;
        std::cout << "[System] Detected " << coreCount << " hardware threads.\n";
        std::cout << "[Loading Screen] Booting permanent worker threads...\n";
        for (int i = 0; i < coreCount; i++)
        {
            worker.push_back(std::thread(workerLoop, i));
        }
    }

    ~ThreadPool()
    {
        for (auto &t : worker)
        {
            if (t.joinable())
            {
                // For now, we forcefully detach them so the program can exit for our test.
                t.detach();
            }
        }
    }
};

int main()
{
    std::cout << "--- MONDAY: THE INFINITE WORKERS ---\n\n";

    ThreadPool enginePool;

    // The main thread is free to run the game loop!
    std::cout << "[Main Thread] Workers are isolated. Proceeding with game loop...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "[Main Thread] Shutting down engine.\n";

    return 0;
}