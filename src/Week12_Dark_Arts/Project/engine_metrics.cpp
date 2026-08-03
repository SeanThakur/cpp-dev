#include <iostream>
#include <chrono>
#include <atomic>
#include <vector>
#include <thread>
#include <functional>

class EngineMetrics
{
private:
    std::atomic<int> totalFrames{0};
    std::atomic<int> totalEntities{0};
    std::atomic<int> peakMemoryMB{0};

public:
    void recordFrame()
    {
        totalFrames.fetch_add(1, std::memory_order_relaxed);
    }
    void addEntities(int count)
    {
        totalEntities.fetch_add(count, std::memory_order_relaxed);
    }

    void reportMemorySpike(int memorySpike)
    {
        int currentPeak = peakMemoryMB.load(std::memory_order_relaxed);
        while (memorySpike > currentPeak)
        {
            if (peakMemoryMB.compare_exchange_weak(currentPeak, memorySpike, std::memory_order_release, std::memory_order_relaxed))
            {
                break;
            }
        }
    }

    void printFinalReport() const
    {
        std::cout << "--- HARDWARE-OPTIMIZED METRICS ---\n";
        std::cout << "Frames Rendered: " << totalFrames.load() << "\n";
        std::cout << "Total Entities:  " << totalEntities.load() << "\n";
        std::cout << "Peak Memory:     " << peakMemoryMB.load() << " MB\n";
    }
};

void engineWorker(EngineMetrics &metrics, int threadId)
{
    for (int i = 0; i < 10000000; i++)
    {
        metrics.recordFrame();
        metrics.addEntities(2);

        // Simulate a massive memory spike every 500 frames.
        // Thread 50 will naturally generate the highest spikes.
        if (i % 500 == 0)
        {
            int simulatedSpike = 100 + (threadId * 15) + (i % 20);
            metrics.reportMemorySpike(simulatedSpike);
        }
    }
}

int main()
{
    std::cout << "[System] Booting 50 lock-free threads...\n\n";
    auto start = std::chrono::high_resolution_clock::now();

    EngineMetrics metrics;
    std::vector<std::thread> swarm;

    for (int i = 1; i <= 50; i++)
    {
        swarm.push_back(std::thread(engineWorker, std::ref(metrics), i));
    }

    for (auto &t : swarm)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    metrics.printFinalReport();
    std::cout << "\n[System] Processed 500,000 operations in " << duration << "ms.\n";

    return 0;
}