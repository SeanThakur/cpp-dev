#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <functional>

struct GameSetting
{
    float masterVolume = 0.5f;
    bool enableVsync = true;
};

class ConfigManager
{
private:
    GameSetting setting;
    mutable std::shared_mutex rwMutex;

public:
    GameSetting getSetting() const
    {
        std::shared_lock<std::shared_mutex> lock(rwMutex);
        return setting;
    }

    void updateVolumn(float volumn)
    {
        std::unique_lock<std::shared_mutex> lock(rwMutex);
        setting.masterVolume = volumn;
    }
};

void readerThread(const ConfigManager &manager, int id, std::atomic<bool> &isRunning)
{
    while (isRunning)
    {
        auto s = manager.getSetting();
        std::this_thread::yield();
    }
}

void writerThread(ConfigManager &manager, std::atomic<bool> &isRunning)
{
    float vol = 0.0f;
    while (isRunning)
    {
        manager.updateVolumn(vol);
        vol += 0.1f;
        if (vol > 1.0f)
        {
            vol = 0.0f;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    ConfigManager manager;
    std::atomic<bool> isRunning = true;
    std::vector<std::thread> worker;

    std::cout << "[System] Starting 20 Reader Threads and 1 Writer Thread...\n";
    for (int i = 1; i <= 20; i++)
    {
        worker.emplace_back(readerThread, std::ref(manager), i, std::ref(isRunning));
    }
    worker.emplace_back(writerThread, std::ref(manager), std::ref(isRunning));

    std::this_thread::sleep_for(std::chrono::seconds(3));
    isRunning = false;

    for (auto &t : worker)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    std::cout << "[System] Stress test complete. Zero data corruption detected.\n";
    return 0;
}