#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int main()
{
    std::cout << "--- WEEK 10 PART 2: THE NON-BLOCKING TICKET ---\n\n";
    std::future<int> simulatePathFinding = std::async(std::launch::async, []()
                                                      {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return 99; });
    std::cout << "[Engine] Background task dispatched. Entering Main Game Loop...\n\n";
    int currentFrame = 1;
    bool enemyMoved = false;
    // 2. THE GAME LOOP (Running at 60 FPS)
    while (currentFrame <= 10)
    {
        std::cout << "--- Frame " << currentFrame << " ---\n";
        std::cout << "  -> Rendering Player Graphics...\n";
        if (!enemyMoved)
        {
            std::future_status status = simulatePathFinding.wait_for(std::chrono::seconds(0));
            if (status == std::future_status::ready)
            {
                int enemeyTarget = simulatePathFinding.get();
                std::cout << "  -> [SUCCESS] Pathfinding data loaded: " << enemeyTarget << "!\n";
                std::cout << "  -> Moving Enemy to target.\n";
                enemyMoved = true;
            }
            else
            {
                std::cout << "  -> [WAITING] Pathfinding still calculating. Enemy stands still.\n";
            }
        }
        // Simulate 16ms of frame time (60 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        currentFrame++;
    }
    std::cout << "\n[Engine] Simulation ended safely.\n";
    return 0;
}