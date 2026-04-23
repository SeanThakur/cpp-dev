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
    {
        auto p1 = std::make_shared<Player>("Sean");
        auto w1 = std::make_shared<Weapon>("Sword");

        p1->myWeapon = w1;
        w1->owner = p1;

        std::cout << "Player use count " << p1.use_count() << " \n";
        std::cout << "Weapon use count " << w1.use_count() << " \n";

        w1->printOwner();
    }
    std::cout << "\n*** LEVEL END ***\n";
    return 0;
}