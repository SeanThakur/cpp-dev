#include <iostream>

class IWeapon
{
public:
    virtual ~IWeapon()
    {
        std::cout << "[-] Weapon is destroyed \n";
    }

    virtual void fire() = 0; // Pure virtual function makes this an interface
};

class PlasmaRifile : public IWeapon
{
public:
    PlasmaRifile()
    {
        std::cout << "[-] Plasma Rifile is created \n";
    }

    ~PlasmaRifile() override
    {
        std::cout << "[-] Plasma Rifile is destroyed \n";
    }

    void fire() override
    {
        std::cout << "--> Plasma Rifle shoots a searing blue beam \n";
    }
};

int main()
{
    {
        std::unique_ptr<IWeapon> myGun = std::make_unique<PlasmaRifile>();
        myGun->fire();
    }
    return 0;
}