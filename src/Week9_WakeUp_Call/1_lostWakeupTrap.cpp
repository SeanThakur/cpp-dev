#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

int main()
{
    std::cout << "--- MONDAY PART 2: THE LOST WAKEUP TRAP ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;

    auto background = [&]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "[Worker] Finally online. Locking and going to sleep...\n";
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        std::cout << "[Worker] I HAVE BEEN AWAKENED!\n"; // Will never print.
    };
    std::thread worker(background);
    std::cout << "[Main] Firing the Wake-Up Ping IMMEDIATELY!\n";
    cv.notify_one();
    std::cout << "[Main] Ping sent. Waiting for worker to finish...\n";
    worker.join();
    return 0;
}