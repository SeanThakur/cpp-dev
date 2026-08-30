#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <deque>
#include <stack>
#include <atomic>
#include <chrono>
#include <functional>

struct ParticleHandle
{
    size_t index;
    int generation;
};

class Particle
{
public:
    float x, y;
    bool inUse = false;
    int currentGeneration = 0;
    std::atomic<bool> lockedByThread{false};
};

class ThreadPool
{
private:
    std::deque<Particle> pool;
    std::stack<size_t, std::vector<size_t>> freeList;
    std::mutex threadMutex;

    int chunkSize;

    void expand()
    {
        size_t oldSize = pool.size();
        for (int i = 0; i < chunkSize; i++)
        {
            pool.emplace_back();
        }
        for (int i = (oldSize + chunkSize) - 1; i >= (int)oldSize; i--)
        {
            freeList.push(i);
        }
    }

public:
    ThreadPool(int size, int expandSize)
    {
        chunkSize = expandSize;
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back();
        }
        for (int i = size - 1; i >= 0; i--)
        {
            freeList.push(i);
        }
    }

    ParticleHandle acquire(float spawnX, float spawnY)
    {
        size_t index;
        int generation;

        {
            std::unique_lock<std::mutex> lock(threadMutex);
            if (freeList.empty())
            {
                expand();
            }

            index = freeList.top();
            freeList.pop();

            pool[index].inUse = true;
            generation = pool[index].currentGeneration;
            lock.unlock();
        }

        pool[index].x = spawnX;
        pool[index].y = spawnY;
        return {index, generation};
    }

    void release(ParticleHandle handle)
    {
        if (handle.generation == -1)
        {
            return;
        }

        std::unique_lock<std::mutex> lock(threadMutex);
        if (!pool[handle.index].inUse || pool[handle.index].currentGeneration != handle.generation)
        {
            return;
        }

        pool[handle.index].inUse = false;
        pool[handle.index].currentGeneration++;
        freeList.push(handle.index);
    }

    Particle *get(ParticleHandle handle)
    {
        if (!pool[handle.index].inUse || pool[handle.index].currentGeneration != handle.generation)
            return nullptr;
        return &pool[handle.index];
    }
};

std::atomic<int> successfulOperations{0};
std::atomic<int> tripwireFailures{0};

void particleWorker(ThreadPool &pool, int threadId)
{
    for (int i = 0; i < 1000; i++)
    {
        // 1. Acquire Memory
        ParticleHandle handle = pool.acquire((float)threadId, (float)i);
        Particle *p = pool.get(handle);

        if (p)
        {
            // 2. TRIPWIRE CHECK
            bool expected = false;
            if (!p->lockedByThread.compare_exchange_strong(expected, true))
            {
                // If we get here, two threads were given the exact same pointer!
                tripwireFailures++;
            }

            // 3. Simulate gameplay work (Hold the pointer)
            p->x += 1.0f;
            p->y += 1.0f;
            std::this_thread::yield();

            // 4. Release the Tripwire
            p->lockedByThread.store(false);

            // 5. Release Memory
            pool.release(handle);
            successfulOperations++;
        }
    }
}

int main()
{
    std::cout << "--- SATURDAY CAPSTONE: THE CONCURRENT SWARM ---\n\n";

    // Start with a tiny pool to force extreme contention and expansion
    ThreadPool masterPool(10, 50);
    std::vector<std::thread> swarm;

    std::cout << "[System] Unleashing 10 threads (10,000 total operations)...\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= 10; i++)
    {
        swarm.push_back(std::thread(particleWorker, std::ref(masterPool), i));
    }

    for (auto &t : swarm)
        t.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\n--- FINAL VERDICT ---\n";
    std::cout << "Time Elapsed:         " << duration << " ms\n";
    std::cout << "Successful Ops:       " << successfulOperations.load() << " / 10000\n";
    std::cout << "Data Race Violations: " << tripwireFailures.load() << " (Expected: 0)\n";

    if (tripwireFailures.load() == 0)
    {
        std::cout << "\n[Engine] PERFECT ISOLATION ACHIEVED. Architecture is bulletproof.\n";
    }
    else
    {
        std::cout << "\n[Engine] CRITICAL FAILURE. Memory overlapped.\n";
    }

    return 0;
}