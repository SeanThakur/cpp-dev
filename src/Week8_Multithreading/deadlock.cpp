#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

int main()
{
    std::mutex mutex1;
    std::mutex mutex2;

    auto worker1 = [&]()
    {
        std::cout << "[Worker 1] Locking Mutex A...\n";
        std::lock_guard<std::mutex> lockA(mutex1);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << "[Worker 1] Holding Mutex A. Now requesting Mutex B...\n";
        std::lock_guard<std::mutex> lockB(mutex2);
        std::cout << "[Worker 1] SUCCESS! Both locks acquired.\n"; // This will never print
    };

    auto worker2 = [&]()
    {
        std::cout << "[Worker 2] Locking Mutex B...\n";
        std::lock_guard<std::mutex> lockB(mutex2);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << "[Worker 2] Holding Mutex B. Now requesting Mutex 1...\n";
        std::lock_guard<std::mutex> lockA(mutex1);
        std::cout << "[Worker 2] SUCCESS! Both locks acquired.\n"; // This will never print
    };
    std::cout << "[Engine] Spawning conflicting threads...\n\n";

    std::thread th1(worker1);
    std::thread th2(worker2);

    th1.join();
    th2.join();

    std::cout << "\n[Engine] All threads joined. Shutting down.\n"; // This will never print

    return 0;
}