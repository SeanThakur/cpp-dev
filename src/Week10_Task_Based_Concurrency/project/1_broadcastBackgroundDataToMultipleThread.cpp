#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <string>

int main()
{
    std::promise<std::string> enginePromise;
    std::shared_future<std::string> sharedFuture = enginePromise.get_future().share();
    std::vector<std::thread> poll;

    auto background = [](std::string systemName, std::shared_future<std::string> ticket)
    {
        std::cout << "  [" << systemName << "] Online. Waiting for level data...\n";
        std::string levelData = ticket.get();
        std::cout << "  [" << systemName << "] Data received (" << levelData << "). Booting!\n";
    };
    poll.push_back(std::thread(background, "Audio", sharedFuture));
    poll.push_back(std::thread(background, "AI", sharedFuture));
    poll.push_back(std::thread(background, "Physics", sharedFuture));

    std::cout << "\n[Engine] Subsystems are waiting. Loading Level from Disk...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "[Engine] Level loaded. Broadcasting to all systems NOW!\n\n";

    enginePromise.set_value("MAP_DUNGEON_01");

    for (auto &threadPoll : poll)
    {
        if (threadPoll.joinable())
        {
            threadPoll.join();
        }
    }
    std::cout << "\n[Engine] All systems booted successfully. Game is live.\n";
    return 0;
}