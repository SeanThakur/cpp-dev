#include <iostream>
#include <shared_mutex>
#include <thread>
#include <chrono>

std::shared_mutex shMutex;
int volume = 100;

void backgroundAudioWriter()
{
    std::unique_lock<std::shared_mutex> lock(shMutex);
    std::cout << "\n  [Audio Thread] Locking exclusively to save new volume settings...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    volume = 80;
    std::cout << "  [Audio Thread] Save complete. Unlocking exclusive shield.\n\n";
}

void uiRenderLooop()
{
    int cacheVolume = volume;
    int frame = 1;
    while (frame <= 10)
    {
        std::cout << "--- UI Frame " << frame << " ---\n";
        std::shared_lock<std::shared_mutex> uiLock(shMutex, std::try_to_lock);
        if (uiLock.owns_lock())
        {
            cacheVolume = volume;
            std::cout << "[UI] Read Lock Acquired! Rendering live volume: " << cacheVolume << "\n";
        }
        else
        {
            std::cout << "[UI] Settings are locked by a writer! Rendering cached volume: " << cacheVolume << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        frame++;
    }
}

int main()
{
    std::cout << "--- WEDNESDAY PART 2: THE NON-BLOCKING READER ---\n\n";
    std::thread ui(uiRenderLooop);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::thread audioWriter(backgroundAudioWriter);
    ui.join();
    audioWriter.join();
    return 0;
}