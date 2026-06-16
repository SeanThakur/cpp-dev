#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>

int main()
{
    std::cout << "--- WEDNESDAY: THE MANUAL PROMISE ---\n\n";
    std::promise<std::string> enginePromise;
    std::future<std::string> engineFuture = enginePromise.get_future();
    std::cout << "[Main] Memory tunnel created. Spawning raw background thread...\n";
    auto background = [](std::promise<std::string> transmitter)
    {
        std::cout << "  [Worker] Transmitter received. Doing heavy string parsing...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string parsedData = "Enemy_Boss_Config_Loaded_Successfully";
        std::cout << "  [Worker] Task complete. Teleporting data through the Promise!\n";
        transmitter.set_value(parsedData);
    };
    // Spawn the thread, moving physical ownership of the promise into it
    std::thread worker(background, std::move(enginePromise));
    std::cout << "[Main] Thread running. Waiting at the Receiver end...\n";
    std::string result = engineFuture.get();
    std::cout << "\n[Main] DATA RECEIVED: " << result << "\n";
    worker.join();
    return 0;
}