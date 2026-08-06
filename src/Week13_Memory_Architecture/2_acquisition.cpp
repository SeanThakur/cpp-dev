#include <iostream>
#include <vector>

class Particle
{
public:
    float x;
    float y;
    bool inUse;

    Particle()
    {
        x = 0.0f;
        y = 0.0f;
        inUse = false;
    }
};

class ParticlePoll
{
private:
    std::vector<Particle> poll;

public:
    ParticlePoll(int size)
    {
        poll = std::vector<Particle>(size);
    }

    Particle *acquire(float spawnX, float spawnY)
    {
        for (size_t i = 0; i < poll.size(); i++)
        {
            if (!poll[i].inUse)
            {
                poll[i].inUse = true;
                poll[i].x = spawnX;
                poll[i].y = spawnY;

                return &poll[i];
            }
        }
        std::cout << "[Warning] Object Pool Exhausted! Cannot spawn particle.\n";
        return nullptr;
    }
};

int main()
{
    std::cout << "--- TUESDAY: THE ACQUISITION ---\n\n";
    ParticlePoll engine(3);
    std::cout << "[Game] Player fired a shotgun! Spawning 3 pellets...\n";

    Particle *p1 = engine.acquire(10.0f, 20.0f);
    Particle *p2 = engine.acquire(12.0f, 22.0f);
    Particle *p3 = engine.acquire(14.0f, 24.0f);

    if (p1)
        std::cout << "  Pellet 1 spawned at X:" << p1->x << " Y:" << p1->y << "\n";
    if (p2)
        std::cout << "  Pellet 2 spawned at X:" << p2->x << " Y:" << p2->y << "\n";
    if (p3)
        std::cout << "  Pellet 3 spawned at X:" << p3->x << " Y:" << p3->y << "\n";

    std::cout << "\n[Game] Player fired again! Spawning 1 more pellet...\n";

    // This will hit our Failsafe because all 3 are inUse!
    Particle *p4 = engine.acquire(16.0f, 26.0f);
    if (!p4)
    {
        std::cout << "  -> Gun jammed. Max particles reached.\n";
    }
    return 0;
}