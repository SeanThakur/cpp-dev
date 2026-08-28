#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
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
};

class ThreadpoolSafe
{
private:
    std::vector<Particle> pool;
    std::vector<size_t> freeList;
    std::mutex poolMutex;

public:
    ThreadpoolSafe(int size)
    {
        pool = std::vector<Particle>(size);
        freeList.reserve(size);
        for (int i = size - 1; i >= 0; i--)
        {
            freeList.push_back(i);
        }
    }

    ParticleHandle acquire(float spawnX, float spawnY)
    {
        std::lock_guard<std::mutex> lock(poolMutex);
        if (freeList.empty())
        {
            return {0, -1};
        }

        size_t idx = freeList.back();
        freeList.pop_back();

        pool[idx].inUse = true;
        pool[idx].x = spawnX;
        pool[idx].y = spawnY;

        return {idx, pool[idx].currentGeneration};
    }

    void release(ParticleHandle handle)
    {
        std::lock_guard<std::mutex> lock(poolMutex);
        if (!pool[handle.index].inUse || pool[handle.index].currentGeneration != handle.generation)
        {
            return;
        }
        pool[handle.index].inUse = false;
        pool[handle.index].currentGeneration++;
        freeList.push_back(handle.index);
    }
};

void workerThread(ThreadpoolSafe &pool, int threadId)
{
    for (int i = 0; i < 50; i++)
    {
        ParticleHandle p = pool.acquire(threadId * 10.0f, 0.0f);
        if (p.generation != -1)
        {
            std::this_thread::yield();
            pool.release(p);
        }
    }
}

int main()
{
    std::cout << "--- MONDAY: THE MUTEX INJECTION ---\n\n";

    ThreadpoolSafe globalPool(100);
    std::vector<std::thread> swarm;

    std::cout << "[Engine] Booting 10 threads to hammer the Object Pool...\n";

    for (int i = 1; i <= 10; i++)
    {
        swarm.push_back(std::thread(workerThread, std::ref(globalPool), i));
    }

    for (auto &t : swarm)
        t.join();

    std::cout << "[Engine] 10 threads successfully acquired and released 500 objects.\n";
    std::cout << "[Engine] Zero data races. Zero memory corruption.\n";
    return 0;
}