#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

int main()
{
    std::cout << "--- THURSDAY: THE BROADCASTER ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;
    bool isBroadcast = false;

    auto background = [&](std::string system)
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "  [" << system << "] Online and standing by...\n";
        cv.wait(lock, [&isBroadcast]
                { return isBroadcast; });
        std::cout << "  [" << system << "] GO SIGNAL RECEIVED! Executing boot sequence.\n";
    };
    std::vector<std::thread> poll;
    std::cout << "[Engine] Booting dormant subsystems...\n";
    poll.push_back(std::thread(background, "Audio"));
    poll.push_back(std::thread(background, "Physics"));
    poll.push_back(std::thread(background, "Rendering"));
    poll.push_back(std::thread(background, "AI_Nav"));
    poll.push_back(std::thread(background, "Network"));

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\n[Engine] All systems are dormant. Preparing to broadcast...\n";
    std::cout << "[Engine] 3... 2... 1... GO!\n\n";

    {
        std::lock_guard<std::mutex> lock(mtx);
        isBroadcast = true;
    }

    cv.notify_all();
    for (auto &threadPoll : poll)
    {
        if (threadPoll.joinable())
        {
            threadPoll.join();
        }
    }
    std::cout << "\n[Engine] Level load complete.\n";
    return 0;
}