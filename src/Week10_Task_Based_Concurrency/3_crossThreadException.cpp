#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <stdexcept>

int main()
{
    std::cout << "---CROSS-THREAD EXCEPTIONS ---\n\n";
    std::promise<std::string> enginePromise;
    std::future<std::string> engineFuture = enginePromise.get_future();

    auto background = [](std::promise<std::string> transmitter)
    {
        std::cout << "  [Worker] Booting up. Attempting to parse corrupted file...\n";
        try
        {
            throw std::runtime_error("CRITICAL: Navmesh file is corrupted! Header missing.");
            transmitter.set_value("SUCCESS: Navmesh Loaded");
        }
        catch (...)
        {
            std::cout << "  [Worker] CAUGHT EXCEPTION! Packaging crash data into Promise...\n";
            transmitter.set_exception(std::current_exception());
        }
    };
    std::thread worker(background, std::move(enginePromise));
    std::cout << "[Engine] Waiting for background data...\n\n";
    try
    {
        std::string result = engineFuture.get();
        std::cout << "[Engine] Data loaded successfully: " << result << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "[Engine] THREAD CRASH DETECTED!\n";
        std::cout << "[Engine] Error Log: " << e.what() << "\n";
        std::cout << "[Engine] Safely falling back to default Navmesh...\n";
    }

    worker.join();
    std::cout << "\n[Engine] Game continues running normally. Disaster averted.\n";

    return 0;
}