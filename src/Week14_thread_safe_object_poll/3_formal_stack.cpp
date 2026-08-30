#include <iostream>
#include <mutex>
#include <vector>
#include <stack>

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

class ThreadPool
{
private:
    std::vector<Particle> pool;
    std::stack<size_t, std::vector<size_t>> freeList;
    std::mutex threadMutex;

public:
    ThreadPool(int size)
    {
        pool = std::vector<Particle>(size);
        std::vector<size_t> initialList;
        initialList.reserve(size);
        for (int i = size - 1; i >= 0; i--)
        {
            initialList.push_back(i);
        }
        freeList = std::stack<size_t, std::vector<size_t>>(std::move(initialList));
    }

    ParticleHandle acquire(float spawnX, float spawnY)
    {
        size_t idx;
        int generation;
        {
            std::unique_lock<std::mutex> lock(threadMutex);
            idx = freeList.top();
            freeList.pop();

            pool[idx].inUse = true;
            generation = pool[idx].currentGeneration;
            lock.unlock();
        }
        pool[idx].x = spawnX;
        pool[idx].y = spawnY;
        return {idx, generation};
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
};

int main()
{
    std::cout << "--- WEDNESDAY: THE FORMAL STACK ---\n\n";

    ThreadPool engineMemory(100);
    ParticleHandle bullet = engineMemory.acquire(50.0f, 50.0f);

    std::cout << "[Engine] Acquired object using strict std::stack semantics.\n";
    std::cout << "[Engine] Underlying memory remains 100% L1-cache friendly.\n";

    return 0;
}