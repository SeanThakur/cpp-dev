#include <iostream>
#include <vector>

class Particle
{
public:
    bool inUse;
    float x, y;
};

class ParticlePoll
{
private:
    std::vector<Particle> poll;
    std::vector<size_t> freeList;

public:
    ParticlePoll(int size)
    {
        poll = std::vector<Particle>(size);
        freeList.reserve(size);

        for (int i = poll.size() - 1; i >= 0; i--)
        {
            freeList.push_back(i);
        }
        std::cout << "[Engine] Pool initialized. Free List loaded with " << freeList.size() << " indices.\n";
    }

    Particle *acquire(float spwanX, float spwanY)
    {
        if (freeList.empty())
        {
            std::cout << "[Warning] Object Pool Exhausted! Cannot spawn particle.\n";
            return nullptr;
        }

        size_t index = freeList.back();
        freeList.pop_back();

        poll[index].inUse = true;
        poll[index].x = spwanX;
        poll[index].y = spwanY;

        return &poll[index];
    }
};

int main()
{
    std::cout << "--- TUESDAY PART 2: THE O(1) FREE LIST ---\n\n";

    ParticlePoll engineParticles(100000);

    // We can now spawn an object from a massive pool in single-digit nanoseconds.
    Particle *bullet = engineParticles.acquire(50.0f, 100.0f);

    if (bullet)
    {
        std::cout << "[Game] Spawned bullet at X:" << bullet->x << " Y:" << bullet->y << "\n";
        std::cout << "[Engine] The CPU did exactly ZERO searching to find this memory.\n";
    }

    return 0;
}