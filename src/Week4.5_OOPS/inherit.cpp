#include <iostream>

class Entity
{
public:
    float x, y;

    void move(float dx, float dy)
    {
        x += dx;
        y += dy;
        std::cout << "Entity moved to (" << x << ", " << y << ")\n";
    }
};

class Player : public Entity
{
public:
    int stamina = 100;

    void sprint()
    {
        stamina -= 10;
        std::cout << "Player sprinted stamina left " << stamina << "\n";
    }
};

int main()
{
    Player hero;
    hero.move(5.0f, 10.0f);
    hero.sprint();
    return 0;
}