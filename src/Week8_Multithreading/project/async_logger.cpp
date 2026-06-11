#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

class AsyncLogger
{
private:
    std::ofstream logFile;
    std::atomic<bool> isRunning;
    std::thread background;
    std::queue<std::string> messageQueue;
    std::mutex messageQueueMutex;

    void processQueue()
    {
        // check if it's running and you have anything in your message queue?
        while (isRunning || !messageQueue.empty())
        {
            std::string messageToWrite = "";
            {
                std::scoped_lock lock(messageQueueMutex);
                messageToWrite = messageQueue.front();
                messageQueue.pop();
            }

            if (messageToWrite != "")
            {
                logFile << messageToWrite << "\n";
            }
            else
            {
                // to prevent 100 percent cpu usage
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

public:
    AsyncLogger(const std::string &filename)
    {
        logFile.open(filename, std::ios::app);
        isRunning = true;
        // Spawn the permanent background worker!
        background = std::thread(&AsyncLogger::processQueue, this);
        if (!logFile.is_open())
        {
            std::cerr << "[CRITICAL] Failed to open log file on hardware!\n";
        }
    }
    ~AsyncLogger()
    {
        isRunning = false;
        if (background.joinable())
        {
            background.join();
        }
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    void log(const std::string &systemName, const std::string &message)
    {
        std::scoped_lock lock(messageQueueMutex);
        messageQueue.push("[" + systemName + "] " + message);
    }
};

int main()
{
    std::cout << "--- SATURDAY TRUE DEEP BUILD: ASYNC LOGGER ---\n\n";
    AsyncLogger asyncLog("async_master_log.txt");
    auto worker = [&asyncLog](int id)
    {
        for (size_t i = 0; i < 5; i++)
        {
            asyncLog.log("System_" + std::to_string(id), "Processed frame " + std::to_string(i));
            // Go right back to doing heavy math! No waiting for the hard drive!
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };
    std::vector<std::thread> pool;
    std::cout << "[Engine] Spawning 10 Systems...\n";

    for (int i = 0; i < 10; i++)
    {
        pool.push_back(std::thread(worker, i));
    }

    for (auto &t : pool)
    {
        t.join();
    }

    std::cout << "[Engine] All subsystems finished. The Logger will now safely shut itself down.\n";
    return 0;
}