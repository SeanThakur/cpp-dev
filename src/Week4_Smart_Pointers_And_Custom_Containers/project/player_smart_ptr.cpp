#include <iostream>
#include <string>
#include <memory>

struct Weapon
{
    std::string name;
    Weapon(std::string n) : name(n)
    {
        std::cout << "--->[+] Weapon is created " << name << "\n";
    }
};

class Player;

class Boss
{
public:
    std::string name;
    std::weak_ptr<Player> aggroTarget;
    Boss(std::string n) : name(n)
    {
        std::cout << "--->[+] Boss is created " << name << "\n";
    }

    void attackTarget();
};

class Player
{
public:
    std::string name;
    std::unique_ptr<Weapon> equippedWeapon;
    std::shared_ptr<Boss> target;
    Player(std::string n) : name(n)
    {
        std::cout << "--->[+] Player is created " << name << "\n";
    }
};

void Boss::attackTarget()
{
    if (std::shared_ptr<Player> target = aggroTarget.lock())
    {
        std::cout << name << " attacks target " << target->name << "! \n";
    }
    else
    {
        std::cout << "target vanished!! \n";
    }
}

int main()
{
    std::cout << "\n*** MAIN Program ***\n";

    std::shared_ptr<Boss> dragon = std::make_shared<Boss>("Black Dragon");
    {
        std::shared_ptr<Player> p1 = std::make_shared<Player>("SEAN");
        p1->equippedWeapon = std::make_unique<Weapon>(Weapon{"Sward"});
        p1->target = dragon;
        dragon->aggroTarget = p1;
        dragon->attackTarget();
    }
    dragon->attackTarget();

    std::cout << "\n*** END Program ***\n";
    return 0;
}