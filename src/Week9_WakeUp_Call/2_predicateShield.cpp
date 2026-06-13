#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

int main()
{
    std::cout << "--- TUESDAY/WEDNESDAY: THE PREDICATE SHIELD ---\n\n";
    std::mutex mtx;
    std::condition_variable cv;
    bool isReady = false;

    auto background = [&]()
    {
        std::cout << "  [Worker] Online. Locking Mutex...\n";
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "  [Worker] Checking state. If false, I will sleep.\n";
        // THE SHIELD: The thread will ONLY wake up (or stay awake) if isReady == true.
        cv.wait(lock, [&isReady]
                { return isReady; });
        std::cout << "  [Worker] AWAKE AND VERIFIED! dataReady is true. Executing logic.\n";
    };
    std::thread worker(background);
    // Notice: NO SLEEP TIMERS. The Main Thread can fire instantly, or in 10 years. It is safe.
    std::cout << "[Main] Setting state to true and firing ping...\n";
    {
        std::lock_guard<std::mutex> lock(mtx);
        isReady = true;
    }
    cv.notify_one();
    worker.join();
    std::cout << "\n[Main] Clean shutdown.\n";
    return 0;
}