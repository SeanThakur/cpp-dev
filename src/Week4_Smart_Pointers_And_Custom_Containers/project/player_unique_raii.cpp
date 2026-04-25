#include <iostream>
#include <memory>
#include <string>

struct Weapon
{
    std::string name;
    int damage;
};

class Player
{
private:
    std::string name;
    int health;
    std::unique_ptr<Weapon> equippedWeapon;

public:
    Player(std::string n, int h) : name(n), health(h)
    {
        equippedWeapon = nullptr;
        std::cout << "[+] Player " << name << " Created \n";
    }

    ~Player()
    {
        std::cout << "[-] Player " << name << " Destroyed \n";
    }

    Player(const Player &other)
    {
        std::cout << "Copy Constructor \n";
        name = other.name;
        health = other.health;
        if (other.equippedWeapon != nullptr)
        {
            equippedWeapon = std::make_unique<Weapon>(*other.equippedWeapon);
        }
    }

    Player &operator=(const Player &other)
    {
        std::cout << "Copy Assignment Constructor \n";
        if (this != &other)
        {
            name = other.name;
            health = other.health;
            if (other.equippedWeapon != nullptr)
            {
                equippedWeapon = std::make_unique<Weapon>(*other.equippedWeapon);
            }
        }

        return *this;
    }

    Player(Player &&other) noexcept
    {
        std::cout << "Move Constructor \n";
        name = std::move(other.name);
        health = other.health;
        if (other.equippedWeapon != nullptr)
        {
            equippedWeapon = std::move(other.equippedWeapon);
        }
    }

    Player &operator=(Player &&other)
    {
        std::cout << "Move Assignment Constructor \n";
        if (this != &other)
        {
            name = std::move(other.name);
            health = other.health;
            if (other.equippedWeapon != nullptr)
            {
                equippedWeapon = std::move(other.equippedWeapon);
            }
            else
            {
                equippedWeapon = nullptr;
            }
        }

        return *this;
    }

    void equip(std::unique_ptr<Weapon> weapon)
    {
        equippedWeapon = std::move(weapon);
        std::cout << "Player " << name << " Equipped weapon " << equippedWeapon->name << "\n";
    }
};

int main()
{
    std::cout << "\n*** MAIN Program ***\n";
    Player hero("Sean", 100);
    std::unique_ptr<Weapon> sward = std::make_unique<Weapon>(Weapon{"Sward", 100});

    hero.equip(std::move(sward));

    Player heroClone = hero;
    Player upgradedHero = std::move(hero);

    std::cout << "\n*** END Program ***\n";
    return 0;
}
