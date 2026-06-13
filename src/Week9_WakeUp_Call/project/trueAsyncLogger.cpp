#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <fstream>
#include <queue>

class TrueAsyncLogger
{
private:
    std::ofstream filelog;
    std::queue<std::string> messageQueue;
    std::mutex queueMtx;
    std::thread background;
    bool engineShuttingDown = false;
    std::condition_variable cv;

    void processQueue()
    {
        while (true)
        {
            std::string messageToWrite = "";
            // 1. Lock the room and wait for the ping
            {
                std::unique_lock<std::mutex> lock(queueMtx);
                // THE PREDICATE SHIELD: Sleep at 0% CPU unless there is work OR the engine is dying
                cv.wait(lock, [this]
                        { return !messageQueue.empty() || engineShuttingDown; });
                // 2. If the engine is shutting down AND the queue is finally empty, break the loop!
                if (engineShuttingDown && messageQueue.empty())
                {
                    break;
                }
                // 3. Extract the message
                messageToWrite = messageQueue.front();
                messageQueue.pop();
            } // Mutex instantly unlocks here!
            if (!messageToWrite.empty() && filelog.is_open())
            {
                filelog << messageToWrite << "\n";
            }
        }
    }

public:
    TrueAsyncLogger(const std::string &filename)
    {
        filelog.open(filename, std::ios::app);
        engineShuttingDown = false;
        background = std::thread(&TrueAsyncLogger::processQueue, this);
    }

    ~TrueAsyncLogger()
    {
        std::cout << "[Logger] Destructor called. Initiating safe shutdown...\n";
        // 1. Flip the Kill Switch safely
        {
            std::lock_guard<std::mutex> lock(queueMtx);
            engineShuttingDown = true;
        }
        // 2. BROADCAST the Kill Switch to wake up the Consumer if it's sleeping
        cv.notify_all();
        // 3. Wait for the Consumer to flush the remaining queue to the hard drive
        if (background.joinable())
        {
            background.join();
        }
        if (filelog.is_open())
        {
            filelog.close();
        }
        std::cout << "[Logger] Hardware file safely closed.\n";
    }

    // THE PRODUCER LOGIC (Game Threads call this. It is blindingly fast.)
    void log(const std::string &system, const std::string &message)
    {
        {
            std::lock_guard<std::mutex> lock(queueMtx);
            messageQueue.push("[" + system + "] " + message);
        }
        // Fire the sniper shot! Wake up the Consumer to process this new message.
        cv.notify_one();
    }
};

int main()
{
    std::cout << "--- SATURDAY DEEP BUILD: TRUE ASYNC LOGGER ---\n\n";
    TrueAsyncLogger engine("true_async_log.txt");
    // Simulate multiple engine subsystems bombarding the logger simultaneously
    auto worker = [&](int id, std::string name)
    {
        for (size_t i = 1; i <= 3; i++)
        {
            engine.log(name, "Operation " + std::to_string(i) + " completed.");
        }
    };
    std::vector<std::thread> poll;
    std::cout << "[Engine] Spawning 5 concurrent subsystems...\n";
    poll.push_back(std::thread(worker, 1, "Physics"));
    poll.push_back(std::thread(worker, 2, "AI"));
    poll.push_back(std::thread(worker, 3, "Rendering"));
    poll.push_back(std::thread(worker, 4, "Network"));
    poll.push_back(std::thread(worker, 5, "Audio"));
    poll.push_back(std::thread(worker, 6, "Vedio"));

    for (auto &threadPoll : poll)
    {
        if (threadPoll.joinable())
        {
            threadPoll.join();
        }
    }
    std::cout << "[Engine] All subsystems finished. Exiting main()...\n";
    return 0;
}