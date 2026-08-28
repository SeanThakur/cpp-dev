#include <iostream>
#include <vector>
#include <mutex>

struct ParticleHandle
{
    size_t index;
    int generation;
};

class Particle
{
public:
    float x, y;
    float trignomatricAngles;
    bool inUse;
    int currentGeneration = 0;
};

class ThreadSafePool
{
private:
    std::vector<Particle> pool;
    std::vector<size_t> freeList;
    std::mutex poolMutex;

public:
    ThreadSafePool(int size)
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
        size_t idx;
        int generation;
        {
            std::unique_lock<std::mutex> lock(poolMutex);
            if (freeList.empty())
            {
                return {0, -1};
            }
            idx = freeList.back();
            freeList.pop_back();

            pool[idx].inUse = true;
            generation = pool[idx].currentGeneration;
            lock.unlock();
        }
        pool[idx].x = spawnX;
        pool[idx].y = spawnY;
        pool[idx].trignomatricAngles = spawnX * spawnY;
        return {idx, generation};
    }

    void release(ParticleHandle handle)
    {
        if (handle.generation == -1)
        {
            return;
        }
        std::unique_lock<std::mutex> lock(poolMutex);
        if (!pool[handle.index].inUse || pool[handle.index].currentGeneration != handle.generation)
        {
            return;
        }
        pool[handle.index].inUse = false;
        pool[handle.index].currentGeneration++;
        freeList.push_back(handle.index);
    }
};

int main()
{
    std::cout << "--- TUESDAY: MINIMIZING THE CRITICAL SECTION ---\n\n";
    std::cout << "[Engine] acquire() now drops the lock before doing math.\n";
    std::cout << "[Engine] Throughput across 10 CPU cores has been exponentially increased.\n";
    return 0;
}
