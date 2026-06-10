#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <thread>

class SafeLogger
{
private:
    std::ofstream logFile;
    std::mutex fileMutex;

public:
    SafeLogger(const std::string &filename)
    {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open())
        {
            std::cerr << "[CRITICAL] Failed to open log file on hardware!\n";
        }
    }

    ~SafeLogger()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    void log(const std::string message, const std::string systemName)
    {
        std::scoped_lock lock(fileMutex);
        if (logFile.is_open())
        {
            logFile << "[" << systemName << "]" << message << "\n";
        }
    }
};

int main()
{
    std::cout << "--- SATURDAY DEEP BUILD: THREAD-SAFE LOGGER ---\n\n";
    std::cout << "[Engine] Booting Master Logger...\n";
    std::vector<std::thread> poll;

    SafeLogger engineLogger("logFile.txt");

    auto worker = [&engineLogger](int threadID, std::string taskName)
    {
        for (size_t i = 1; i <= 5; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::string message = "Completed Cycle " + std::to_string(i) + " successfully.";
            engineLogger.log(message, "Worker_" + std::to_string(threadID) + " (" + taskName + ")");
        }
    };

    std::cout << "[Engine] Spawning 10 concurrent subsystems...\n";

    for (size_t i = 0; i < 10; i++)
    {
        std::string tasktype = i % 2 == 0 ? "Physics" : "AI_Pathfinding";
        poll.push_back(std::thread(worker, i, tasktype));
    };

    for (auto &threadPoll : poll)
    {
        if (threadPoll.joinable())
        {
            threadPoll.join();
        }
    }

    std::cout << "[Engine] All subsystems finished executing.\n";
    std::cout << "[Engine] Check 'engine_master_log.txt' in your folder to verify the safe formatting.\n";
    return 0;
}