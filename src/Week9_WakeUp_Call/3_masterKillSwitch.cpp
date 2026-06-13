#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

int main()
{
    std::cout << "--- THURSDAY PART 2: THE MASTER KILL SWITCH ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;
    bool hasWork = false;
    bool engineShutDown = false;
    auto background = [&]()
    {
        std::cout << "  [Worker] Online. Entering infinite loop...\n";
        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return hasWork || engineShutDown; });
            if (engineShutDown)
            {
                std::cout << "  [Worker] Kill Switch detected. Terminating thread safely.\n";
                break;
            }
            std::cout << "  [Worker] Processing task...\n";
            hasWork = false;
        }
    };
    std::thread worker(background);
    // Give it time to fall asleep
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "\n[Main] Initiating Engine Shutdown...\n";
    {
        std::lock_guard<std::mutex> lock(mtx);
        engineShutDown = true;
    }
    // You MUST broadcast the kill switch to wake up any threads trapped in a coma!
    cv.notify_all();
    worker.join();
    std::cout << "[Main] Engine shutdown complete.\n";
    return 0;
}