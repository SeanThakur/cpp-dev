#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

struct BulletHandle
{
    size_t index;
    int generation;
};

class Bullet
{
public:
    float x, y, speed;
    bool inUse;
    int currentGeneration;
};

class BulletPoll
{
private:
    std::vector<Bullet> poll;
    std::vector<size_t> freeList;

public:
    BulletPoll(int size)
    {
        poll = std::vector<Bullet>(size);
        freeList.reserve(size);

        for (int i = size - 1; i >= 0; i--)
        {
            freeList.push_back(i);
        }
    }

    BulletHandle acquire(float spawnX, float spawnY)
    {
        if (freeList.empty())
        {
            return {0, -1}; // Gun Jam!
        }
        size_t index = freeList.back();
        freeList.pop_back();

        poll[index].inUse = true;
        poll[index].x = spawnX;
        poll[index].y = spawnY;
        poll[index].speed = 10.0f; // Move up 10 units per frame

        return {index, poll[index].currentGeneration};
    }

    void release(BulletHandle handle)
    {
        if (!poll[handle.index].inUse || poll[handle.index].currentGeneration != handle.generation)
        {
            return;
        }
        poll[handle.index].inUse = false;
        poll[handle.index].currentGeneration++;
        freeList.push_back(handle.index);
    }

    Bullet *get(BulletHandle handle)
    {
        if (!poll[handle.index].inUse || poll[handle.index].currentGeneration != handle.generation)
        {
            return nullptr;
        }
        return &poll[handle.index];
    }
};

int main()
{
    std::cout << "--- CAPSTONE: BULLET HELL ---\n\n";
    // 1. THE LOADING SCREEN
    // We only have enough RAM for 100 concurrent bullets.
    BulletPoll engineMemory(100);
    std::vector<BulletHandle> activeBullets;
    activeBullets.reserve(200);

    // 2. THE GAME LOOP
    int framesSimulated = 25;
    for (int frame = 0; frame <= framesSimulated; frame++)
    {
        std::cout << "[Frame " << frame << "]\n";
        // --- FIRING PHASE ---
        // Fire 10 bullets this frame
        int spawnedThisFrame = 0;
        for (int i = 0; i < 10; i++)
        {
            BulletHandle handle = engineMemory.acquire(50.0f, 0.0f); // Spawn at bottom of screen (Y=0)
            if (handle.generation != -1)
            {
                activeBullets.push_back(handle);
                spawnedThisFrame++;
            }
        }
        if (spawnedThisFrame < 10)
        {
            std::cout << "  -> WARNING: Gun Jammed! Out of memory. Fired " << spawnedThisFrame << "/10\n";
        }
        else
        {
            std::cout << "  -> Fired 10 bullets.\n";
        }
        // --- PHYSICS UPDATE PHASE ---
        int bulletsRecycled = 0;
        for (size_t i = 0; i <= activeBullets.size(); i++)
        {
            Bullet *b = engineMemory.get(activeBullets[i]);
            if (b != nullptr)
            {
                // Move bullet up
                b->y += b->speed;
                // If the bullet goes off-screen (Y >= 100), it dies!
                if (b->y >= 100.0f)
                {
                    engineMemory.release(activeBullets[i]);
                    bulletsRecycled++;
                }
            }
        }
        if (bulletsRecycled > 0)
        {
            std::cout << "  -> " << bulletsRecycled << " bullets went off-screen and were instantly recycled.\n";
        }

        std::cout << "-----------------------------------\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate time passing
    }
    return 0;
}