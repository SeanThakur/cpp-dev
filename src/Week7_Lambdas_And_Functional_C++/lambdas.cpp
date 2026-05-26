#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

struct Entity
{
    std::string name;
    int health;
    bool isHostile;
};

int main()
{
    std::cout << "**Main Program**\n";
    std::cout << "\n\n******START OF Sortable Programm Example******\n\n";
    std::vector<int> numbers = {45, 12, 89, 33, 7, 99, 21, 56, 4, 78};
    std::sort(numbers.begin(), numbers.end());
    std::cout << "Original Sorting algo \n";
    for (const int &num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n\n";
    std::sort(numbers.begin(), numbers.end(), [](int a, int b)
              { return a > b; });
    std::cout << "Lambdas Sorting algo \n";
    for (const int &num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n\n******END Of Sortable Programm Example******\n\n";
    std::cout << "\n\n******START OF Entity Programm Example******\n\n";
    std::vector<Entity> world = {
        {"Human", 200, true},
        {"Dragon", 2000, false},
        {"Goblin", 40, true},
        {"Elif", 900, false},
    };

    auto isDead = [](const Entity &e)
    {
        return e.isHostile;
    };

    auto lowestHealth = [](const Entity &e)
    {
        return e.health < 50;
    };

    int enemyCount = std::count_if(world.begin(), world.end(), isDead);
    std::cout << "[Scanner] Detected " << enemyCount << " hostile entities in the area.\n";
    auto target = std::find_if(world.begin(), world.end(), lowestHealth);
    std::cout << "[Scanner] Detected " << target->name << " lowest level entities in the area.\n";
    std::cout << "\n\n******END OF Entity Programm Example******\n\n";
    return 0;
}