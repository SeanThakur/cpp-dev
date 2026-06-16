#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>

int main()
{
    std::cout << "--- WEDNESDAY PART 2: THE BULLETPROOF PROMISE ---\n\n";
    std::promise<std::string> enginePromise;
    std::future<std::string> engineFuture = enginePromise.get_future();

    auto background = [](std::promise<std::string> transimiter, bool fileExists)
    {
        std::string message;
        if (!fileExists)
        {
            std::cout << "  [Worker] ERROR: File not found. Aborting operation early.\n";
            message = "FAIL: Missing File";
        }
        else
        {
            std::cout << "  [Worker] File found. Parsing data...\n";
            message = "SUCCESS: AI_Navmesh_Loaded";
        }
        std::cout << "  [Worker] Sealing tunnel and teleporting data...\n";
        transimiter.set_value(message);
    };

    std::thread worker(background, std::move(enginePromise), false);
    std::cout << "[Main] Waiting for background thread to report status...\n";
    std::string status = engineFuture.get();
    std::cout << "\n[Main] Status Received: " << status << "\n";
    if (status.find("FAIL") != std::string::npos)
    {
        std::cout << "[Main] Fallback protocol initiated. Spawning basic AI.\n";
    }
    worker.join();
    return 0;
}