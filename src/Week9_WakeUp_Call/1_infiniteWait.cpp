#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

int main()
{
    std::cout << "--- WEEK 9 MONDAY: THE INFINITE WAIT ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;

    auto background = [&]()
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "[Worker] Lock acquired. No work found. Going into a deep sleep (0% CPU)...\n";
        cv.wait(lock);
        std::cout << "[Worker] I HAVE BEEN AWAKENED!\n";
    };
    std::cout << "[Main] Spawning background thread...\n\n";
    std::thread worker(background);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\n[Main] The Main Thread is finished, but the Worker is still in a coma.\n";
    std::cout << "[Main] Because we never wake it up, the program will freeze here forever.\n";

    worker.join();
    return 0;
}