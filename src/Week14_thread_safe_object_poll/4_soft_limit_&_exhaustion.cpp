#include <iostream>
#include <vector>
#include <mutex>
#include <stack>
#include <deque>
#include <thread>

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

class ElasticPool
{
private:
    std::deque<Particle> pool;
    std::stack<size_t, std::vector<size_t>> freeList;
    std::mutex elasticMutex;
    int chunkSize;

    void expandPool()
    {
        std::cout << "[Engine Warning] Pool exhausted! Soft Limit triggered. Expanding by " << chunkSize << " objects...\n";
        size_t oldSize = pool.size();
        for (int i = 0; i < chunkSize; i++)
        {
            pool.push_back(Particle());
        }
        for (int i = (oldSize + chunkSize) - 1; i >= (int)oldSize; i--)
        {
            freeList.push(i);
        }
    }

public:
    ElasticPool(int size, int expandSize)
    {
        chunkSize = expandSize;
        for (int i = 0; i < size; i++)
        {
            pool.push_back(Particle());
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
            std::unique_lock<std::mutex> lock(elasticMutex);

            // THE DECISION POINT
            if (freeList.empty())
            {
                // If this was a Hard Limit, we would return {0, -1} here.
                // Because it is a Soft Limit, we expand the pool on the fly.
                expandPool();
            }

            index = freeList.top();
            freeList.pop();

            pool[index].inUse = true;
            generation = pool[index].currentGeneration;
            lock.unlock(); // Drop lock before math
        }

        pool[index].x = spawnX;
        pool[index].y = spawnY;
        return {index, generation};
    }

    void release(ParticleHandle handle)
    {
        if (handle.generation == -1)
            return;

        std::unique_lock<std::mutex> lock(elasticMutex);
        if (!pool[handle.index].inUse || pool[handle.index].currentGeneration != handle.generation)
            return;

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

int main()
{
    std::cout << "--- THURSDAY: SOFT LIMITS & EXHAUSTION ---\n\n";

    // Initial pool of 2, but expands by 5 if it runs out
    ElasticPool engine(2, 5);

    std::cout << "[Game] Spawning 3 boss missiles...\n";

    ParticleHandle m1 = engine.acquire(10.0f, 10.0f);
    ParticleHandle m2 = engine.acquire(20.0f, 20.0f);

    // The pool only has 2 slots. This 3rd acquire will trigger the Soft Limit.
    ParticleHandle m3 = engine.acquire(30.0f, 30.0f);

    std::cout << "[Engine] Successfully survived exhaustion and spawned missile 3 at X:"
              << engine.get(m3)->x << "\n";
    return 0;
}