#include <iostream>
#include <memory>

class IEntity
{
public:
    virtual ~IEntity()
    {
        std::cout << "[-] IEntity Destroyed \n";
    }
    virtual void update() = 0;
};

class Player : public IEntity
{
public:
    ~Player() override
    {
        std::cout << "[-] Player Destroyed \n";
    }

    void update() override
    {
        std::cout << "[Player] Scanning for enemies... \n";
    }
};

class Goblin : public IEntity
{
public:
    ~Goblin() override
    {
        std::cout << "[-] Goblin despawned \n";
    }

    void update() override
    {
        std::cout << "[Goblin] Patrolling the cave... \n";
    }
};

int main()
{
    std::cout << "\n *** MAIN PROGRAMM ***\n";
    std::vector<std::unique_ptr<IEntity>> gameWorld;
    gameWorld.push_back(std::make_unique<Player>());
    gameWorld.push_back(std::make_unique<Goblin>());
    gameWorld.push_back(std::make_unique<Goblin>());

    for (auto &game : gameWorld)
    {
        game->update();
    }

    std::cout << "\n *** END PROGRAMM ***\n";
    return 0;
}