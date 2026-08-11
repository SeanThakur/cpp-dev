#include <iostream>
#include <vector>
#include <chrono>

// ==========================================
// THE ENTITY
// ==========================================
class Particle
{
public:
    float x, y;
    bool inUse = false;
    Particle() : x(0), y(0) {}
};

// ==========================================
// THE O(1) OBJECT POOL
// ==========================================
class ParticlePool
{
private:
    std::vector<Particle> pool;
    std::vector<size_t> freeList;

public:
    ParticlePool(int size)
    {
        pool = std::vector<Particle>(size);
        freeList.reserve(size);
        for (int i = size - 1; i >= 0; i--)
            freeList.push_back(i);
    }
    Particle *acquire()
    {
        if (freeList.empty())
            return nullptr;
        size_t index = freeList.back();
        freeList.pop_back();
        pool[index].inUse = true;
        return &pool[index];
    }
    void release(Particle *p)
    {
        size_t index = p - pool.data();
        pool[index].inUse = false;
        freeList.push_back(index);
    }
};

int main()
{
    std::cout << "--- THURSDAY: THE BENCHMARK ---\n\n";

    const int OBJECT_COUNT = 100000;
    std::vector<Particle *> standardPointers;
    standardPointers.reserve(OBJECT_COUNT);

    std::vector<Particle *> poolPointers;
    poolPointers.reserve(OBJECT_COUNT);

    // ==========================================
    // TEST 1: THE OPERATING SYSTEM (new/delete)
    // ==========================================
    auto startOS = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        standardPointers.push_back(new Particle()); // OS Lock and Allocation
    }
    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        delete standardPointers[i]; // OS Free
    }

    auto endOS = std::chrono::high_resolution_clock::now();
    auto durationOS = std::chrono::duration_cast<std::chrono::microseconds>(endOS - startOS).count();

    // ==========================================
    // TEST 2: THE CUSTOM ENGINE (acquire/release)
    // ==========================================
    // We pre-allocate outside the timer, simulating the Loading Screen!
    ParticlePool enginePool(OBJECT_COUNT);

    auto startPool = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        poolPointers.push_back(enginePool.acquire()); // O(1) Pop
    }
    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        enginePool.release(poolPointers[i]); // O(1) Math & Push
    }

    auto endPool = std::chrono::high_resolution_clock::now();
    auto durationPool = std::chrono::duration_cast<std::chrono::microseconds>(endPool - startPool).count();

    // ==========================================
    // THE VERDICT
    // ==========================================
    std::cout << "[Verdict] Spawning & Destroying " << OBJECT_COUNT << " objects:\n";
    std::cout << "  -> Standard 'new/delete': " << durationOS << " microseconds\n";
    std::cout << "  -> Engine Object Pool:    " << durationPool << " microseconds\n\n";

    if (durationPool > 0)
    {
        std::cout << "The Object Pool is " << (durationOS / durationPool) << "x faster.\n";
    }

    return 0;
}