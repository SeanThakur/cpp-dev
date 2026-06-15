#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int main()
{
    std::cout << "--- WEEK 10: THE ASYNC LAUNCH ---\n\n";
    auto backgroundPathFinding = []() -> int
    {
        std::cout << "  [Background Core] Calculating complex A* pathfinding...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "  [Background Core] Calculation complete! Teleporting data.\n";
        return 42;
    };
    std::cout << "[Main] Dispatching Pathfinding task to a background core...\n";
    std::future<int> resultPathFinding = std::async(std::launch::async, backgroundPathFinding);
    std::cout << "[Main] Task dispatched! I am free to do other work.\n";
    std::cout << "[Main] Rendering UI...\n";
    std::cout << "[Main] Playing Audio...\n";
    // Wait for 1 second just to prove they are running simultaneously
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\n[Main] Okay, the enemy needs to move NOW. I need that data.\n";
    std::cout << "[Main] Calling .get() on the Ticket...\n";
    int finalCoords = resultPathFinding.get();
    std::cout << "\n[Main] Data received: " << finalCoords << ". Updating enemy position.\n";
    return 0;
}