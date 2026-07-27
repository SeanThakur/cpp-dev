#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

int payload = 0;
std::atomic<bool> ready(false);

void generateData()
{
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    payload = 11;
    ready.store(true, std::memory_order_seq_cst);
}

void consumeData()
{
    while (!ready.load(std::memory_order_seq_cst))
    {
        std::this_thread::yield();
    }

    std::cout << "[Engine] Consumer read payload: " << payload << "\n";
}

int main()
{
    std::cout << "--- WEDNESDAY: SEQUENTIAL CONSISTENCY ---\n\n";
    std::thread t1(generateData);
    std::thread t2(consumeData);

    t1.join();
    t2.join();
    return 0;
}