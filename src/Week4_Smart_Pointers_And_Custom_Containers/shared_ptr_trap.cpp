#include <iostream>
#include <memory>
#include <string>

class Weapon;

class Player
{
public:
    std::string name;
    std::shared_ptr<Weapon> myWeapon;

    Player(std::string n) : name(n)
    {
        std::cout << "[+] Player is Created " << name << "\n";
    }

    ~Player()
    {
        std::cout << "[-] Player is Destroyed " << name << "\n";
    }
};

class Weapon
{
public:
    std::string type;
    // std::shared_ptr<Player> owner; // This will create a circular reference and cause a memory leak
    std::weak_ptr<Player> owner; // Trap solution: Use weak_ptr to break the circular reference
    // Why? because a weapon can exist without an owner, but a player cannot exist without a weapon.
    // So the weapon should not own the player, but the player should own the weapon.

    void printOwner()
    {
        if (std::shared_ptr<Player> tempOwner = owner.lock()) // lock returns a shared_ptr if the object is still alive, otherwise it returns nullptr
        {
            std::cout << "Weapon " << type << " is owned by " << tempOwner->name << "\n";
        }
        else
        {
            std::cout << "Weapon " << type << " has no owner\n";
        }
    }

    Weapon(std::string t) : type(t)
    {
        std::cout << "[+] Weapon Forged: " << type << "\n";
    }
    ~Weapon()
    {
        std::cout << "[-] Weapon Destroyed: " << type << "\n";
    }
};

int main()
{
    std::cout << "\n*** LEVEL START ***\n";
    // This code will show the weak pointer solution to the circular reference problem.
    // If we use shared_ptr for both Player and Weapon, they will keep each other alive and never get destroyed,
    // causing a memory leak. By using weak_ptr for the owner in Weapon, we can break the circular reference and allow
    // both Player and Weapon to be destroyed properly when they go out of scope.
    // {
    //     auto p1 = std::make_shared<Player>("Sean");
    //     auto w1 = std::make_shared<Weapon>("Sword");

    //     p1->myWeapon = w1;
    //     w1->owner = p1;

    //     std::cout << "Player use count " << p1.use_count() << " \n";
    //     std::cout << "Weapon use count " << w1.use_count() << " \n";

    //     w1->printOwner();
    // }
    // Below code will show the same thing but with a more realistic scenario
    // where the weapon is created first and then assigned to the player.
    // This will also demonstrate that the weapon can exist without an owner,
    // but the player cannot exist without a weapon.
    auto sword = std::make_shared<Weapon>("Sword");
    {
        auto player = std::make_shared<Player>("Sean");
        player->myWeapon = sword;
        sword->owner = player;

        std::cout << "Player use count " << player.use_count() << " \n";
        std::cout << "Weapon use count " << sword.use_count() << " \n";

        sword->printOwner();
    }
    std::cout << "\n*** LEVEL END ***\n";
    return 0;
}