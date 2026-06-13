#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

int main()
{
    std::cout << "--- TUESDAY: THE PING ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;
    auto background = [&]()
    {
        std::cout << "  [Worker] Booting up...\n";
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "  [Worker] Lock acquired. Going into 0% CPU sleep...\n";
        cv.wait(lock);
        std::cout << "  [Worker] PING RECEIVED! Resuming execution.\n";
    };
    std::cout << "[Main] Spawning background worker...\n";
    std::thread worker(background);
    std::cout << "[Main] Simulating 2 seconds of heavy Engine Math...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[Main] Work complete. Firing the Wake-Up Ping!\n";
    cv.notify_one();
    worker.join();
    std::cout << "\n[Main] Worker successfully joined. Shutting down.\n";
    return 0;
}