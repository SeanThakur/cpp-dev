#include <iostream>
#include <memory>

class IWeapon
{
public:
    virtual ~IWeapon()
    {
        std::cout << "Weapon is destroyed \n";
    }
    virtual void fire() = 0;
};

class Shotgun : public IWeapon
{
public:
    ~Shotgun() override
    {
        std::cout << "--> Shotgun destroyed \n";
    }
    void fire() override
    {
        std::cout << "--> Shotgun blasts a spread of pellets! *BOOM* \n";
    }
};

class Sniper : public IWeapon
{
public:
    ~Sniper() override
    {
        std::cout << "--> Sniper destroyed \n";
    }
    void fire() override
    {
        std::cout << "--> Sniper fires a high-velocity round! *CRACK* \n";
    }
};

class Player
{
private:
    std::unique_ptr<IWeapon> currentWeapon;

public:
    void equipWeapon(std::unique_ptr<IWeapon> weapon)
    {
        currentWeapon = std::move(weapon);
    }

    void pullTrigger()
    {
        if (currentWeapon != nullptr)
        {
            currentWeapon->fire();
        }
        else
        {
            std::cout << "Player is unarmed!! \n";
        }
    }
};

int main()
{
    Player player;
    player.pullTrigger();
    player.equipWeapon(std::make_unique<Shotgun>());
    player.pullTrigger();
    player.equipWeapon(std::make_unique<Sniper>());
    player.pullTrigger();
    return 0;
}