#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <stdexcept>

int main()
{
    std::packaged_task<std::string(bool)> engineTask([](bool forcedCrash)
                                                     {
        std::cout << "  [Worker] Executing complex physics math...\n";
        if(forcedCrash) {
            throw std::runtime_error("FATAL: Physics Division by Zero!");
        }
        // Notice: NO set_value() here! 
        // The packaged_task handles it automatically when we return!
        return std::string("SUCCESS: Collision Calculated"); });
    std::future<std::string> liveFuture = engineTask.get_future();
    std::cout << "[Engine] Task Packaged. Spawning heavily controlled thread...\n";
    std::thread worker(std::move(engineTask), false);
    try
    {
        std::cout << "[Engine] Waiting for physics result...\n";
        std::string result = liveFuture.get();
        std::cout << "[Engine] Result: " << result << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n[Engine] CRASH INTERCEPTED SAFELY: " << e.what() << "\n";
        std::cout << "[Engine] Falling back to simplified collision.\n";
    }

    worker.join();
    return 0;
}