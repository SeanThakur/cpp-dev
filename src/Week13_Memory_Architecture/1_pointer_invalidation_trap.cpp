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
    std::vector<Particle> container;

public:
    ParticlePoll(int pollSize)
    {
        container.reserve(pollSize);
    }

    void printStatus() const
    {
        std::cout << "[ParticlePool] Fortress secured. Total Capacity " << container.capacity() << " Total Size " << container.size() << " Total memory locked: "
                  << container.size() * sizeof(Particle) << " bytes.\n";
    }
};

int main()
{
    std::cout << "--- MONDAY PART 2: THE POINTER INVALIDATION TRAP ---\n\n";
    ParticlePoll enginePoll(1000);
    enginePoll.printStatus();
    return 0;
}