#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

std::atomic<int> counter{0};

void playerShooting(int threadId)
{
    for (int i = 0; i < 100000; i++)
    {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main()
{
    std::cout << "--- THURSDAY: RELAXED MEMORY ---\n\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> poll;
    for (int i = 0; i < 20; i++)
    {
        poll.emplace_back(std::thread(playerShooting, i));
    }
    for (auto &t : poll)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[Engine] Simulation Complete in " << duration << "ms.\n";
    std::cout << "[Engine] Total Bullets Fired: " << counter.load() << " (Expected: 2000000)\n";
    return 0;
}