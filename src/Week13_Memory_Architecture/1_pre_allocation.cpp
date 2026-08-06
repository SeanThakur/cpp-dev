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

int main()
{
    std::cout << "--- MONDAY: THE DORMANT ARRAY ---\n\n";
    std::cout << "[Loading Screen] Requesting memory from OS...\n";
    std::vector<Particle> pollParticle(1000);

    std::cout << "[Engine] Memory allocated successfully.\n";
    std::cout << "[Engine] Pool Size: " << pollParticle.size() << " And Capacity: " << pollParticle.capacity() << " particles.\n";
    std::cout << "[Engine] Particle 0 status -> inUse: "
              << (pollParticle[0].inUse ? "TRUE" : "FALSE") << "\n\n";
    std::cout << "[Engine] Level Started. Entering Main Loop...\n";
    return 0;
}