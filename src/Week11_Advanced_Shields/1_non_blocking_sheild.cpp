#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

int playerHealth = 100;
std::mutex playerMutex;

void phyicsThread()
{
    std::unique_lock<std::mutex> lock(playerMutex);
    std::cout << "  [Physics] Calculating massive fall damage. Memory locked...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    playerHealth -= 40;
    std::cout << "  [Physics] Damage applied. Unlocking memory.\n";
}

void uiRenderThread()
{
    int frame = 1;
    while (frame <= 10)
    {
        std::cout << "--- UI Frame " << frame << " ---\n";
        std::unique_lock<std::mutex> lock(playerMutex, std::try_to_lock);
        if (lock.owns_lock())
        {
            std::cout << "  [UI] Mutex acquired! Rendering updated health: " << playerHealth << "\n";
        }
        else
        {
            std::cout << "  [UI] Mutex is busy! Rendering cached health. Skipping update to maintain FPS.\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        frame++;
    }
}

int main()
{
    std::cout << "--- MONDAY PART 2: THE NON-BLOCKING SHIELD ---\n\n";
    std::thread physic(phyicsThread);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::thread UI(uiRenderThread);

    physic.join();
    UI.join();
    return 0;
}