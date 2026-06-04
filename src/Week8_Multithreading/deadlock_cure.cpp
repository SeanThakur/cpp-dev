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
        std::cout << "[Worker 1] Requesting Mutex A and B simultaneously...\n";
        std::scoped_lock lock(mutex1, mutex2);
        std::cout << "[Worker 1] SUCCESS! Both locks acquired safely.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };

    auto worker2 = [&]()
    {
        std::cout << "[Worker 2] Requesting Mutex A and B simultaneously...\n";
        std::scoped_lock lock(mutex1, mutex2);
        std::cout << "[Worker 2] SUCCESS! Both locks acquired safely.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };
    std::cout << "[Engine] Spawning protected threads...\n\n";

    std::thread th1(worker1);
    std::thread th2(worker2);

    th1.join();
    th2.join();
    std::cout << "\n[Engine] All threads joined. System shut down perfectly.\n";

    return 0;
}