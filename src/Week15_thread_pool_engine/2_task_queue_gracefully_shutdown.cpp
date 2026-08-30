#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::atomic<bool> stop{false};

    void workerLoop(int workerID)
    {
        while (true)
        {
            std::function<void()> currentTask;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCV.wait(lock, [this]()
                             { return stop.load() || !tasks.empty(); });

                if (stop.load() && tasks.empty())
                {
                    std::cout << "[Worker " << workerID << "] Shutting down cleanly.\n";
                    return;
                }
                currentTask = tasks.front();
                tasks.pop();
            }
            currentTask();
        }
    }

public:
    ThreadPool()
    {
        unsigned int coreCount = std::thread::hardware_concurrency();
        if (coreCount == 0)
        {
            coreCount = 4;
        }
        for (unsigned int i = 0; i < coreCount; i++)
        {
            workers.push_back(std::thread(&ThreadPool::workerLoop, this, i));
        }
    }

    void submit(std::function<void()> newTask)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push(newTask);
        }
        queueCV.notify_one();
    }

    ~ThreadPool()
    {
        std::cout << "\n[Engine] Initiating graceful shutdown...\n";
        stop.store(true);
        queueCV.notify_all();
        for (auto &t : workers)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
        std::cout << "[Engine] All threads joined. Memory secured.\n";
    }
};

int main()
{
    std::cout << "--- GRACEFUL SHUTDOWN ---\n\n";

    {
        ThreadPool enginePool;

        enginePool.submit([]()
                          {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::cout << "   -> [Task] Finished saving game data.\n"; });

        // The enginePool goes out of scope here, triggering the destructor!
    }

    std::cout << "[OS] Program terminated safely.\n";
    return 0;
}