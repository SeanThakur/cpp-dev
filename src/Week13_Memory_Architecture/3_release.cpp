#include <iostream>
#include <vector>

class Particle
{
public:
    float x, y;
    bool inUse = false;
};

class ParticlePoll
{
private:
    std::vector<Particle> poll;
    std::vector<size_t> freeList;

public:
    ParticlePoll(int pollSize)
    {
        poll = std::vector<Particle>(pollSize);
        freeList.reserve(pollSize);
        for (int i = pollSize - 1; i >= 0; i--)
        {
            freeList.push_back(i);
        }
    }

    Particle *acquire(float spawnX, float spawnY)
    {
        if (freeList.empty())
        {
            return nullptr;
        }
        size_t index = freeList.back();
        freeList.pop_back();
        poll[index].x = spawnX;
        poll[index].y = spawnY;
        poll[index].inUse = true;

        return &poll[index];
    }

    void release(Particle *p)
    {
        if (p == nullptr)
        {
            return;
        }
        // Imagine pool.data() sits at memory address 0x1000.
        // Each Particle is exactly 8 bytes (two 4-byte floats, plus boolean padding).
        // If the pointer p points to address 0x1010, the CPU does the math:
        //(0x1010 - 0x1000) = 16 bytes.
        // Divide by 8 bytes per Particle... and it instantly knows you are trying to delete index 2
        size_t index = p - poll.data();
        if (index < 0 || index >= poll.size())
        {
            std::cout << "OUT OF BOUND \n\n";
            return;
        }
        if (!poll[index].inUse)
        {
            std::cout << "Ignored it's already destroyed! \n\n";
            return;
        }

        poll[index].inUse = false;
        freeList.push_back(index);
    }
};

int main()
{
    std::cout << "--- WEDNESDAY: THE RELEASE ---\n\n";
    ParticlePoll engineParticles(5);

    std::cout << "[Game] Player fired a bullet!\n";
    Particle *bullet = engineParticles.acquire(100.0f, 50.0f);

    std::cout << "[Game] Bullet hit a wall. Recycling memory...\n";

    // We "delete" the bullet without asking the OS!
    engineParticles.release(bullet);

    std::cout << "[Engine] Memory successfully recycled in O(1) time.\n";

    // Attempting a double-delete to test our failsafe
    std::cout << "\n[Game] Collision system accidentally tried to delete the bullet again:\n";
    engineParticles.release(bullet);
    return 0;
}