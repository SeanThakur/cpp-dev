#include <iostream>

class Entity
{
public:
    virtual void attack()
    {
        std::cout << "Entity does a boring punch.\n";
    }
};

class Player : public Entity
{
public:
    void attack() override
    {
        std::cout << "Player slashes with a glowing sword!\n";
    }
};

void safeRef(Entity &e)
{
    std::cout << "Safe Reference \n";
    e.attack();
}

int main()
{
    Player p1;
    Entity *basePtr = &p1;
    basePtr->attack(); // Calls Player's attack due to virtual function
    safeRef(p1);       // Calls Player's attack due to virtual function
    return 0;
}