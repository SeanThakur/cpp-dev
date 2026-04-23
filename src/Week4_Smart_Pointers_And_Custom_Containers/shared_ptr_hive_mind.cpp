#include <iostream>
#include <memory>
#include <string>

class MissionData
{
public:
    std::string objective;

    MissionData(std::string data) : objective(data)
    {
        std::cout << "Mission Data Created: " << objective << "\n";
    }

    ~MissionData()
    {
        std::cout << "[-] Mission Data Erased from RAM!\n";
    }
};

class Soldier
{
private:
    std::string name;
    std::shared_ptr<MissionData> intel;

public:
    Soldier(std::string n, std::shared_ptr<MissionData> data) : name(n), intel(data)
    {
        std::cout << "[+] Soldier " << name << " has joined the squad and received the intel \n";
    }

    void readIntel()
    {
        std::cout << "--> Soldier " << name << " reading the objective " << intel->objective << " \n";
    }

    void updateIntel(std::string newObjective)
    {
        std::cout << "***--> Soldier " << name << " is Updated the global intel ****\n";
        intel->objective = newObjective;
    }
};

int main()
{
	std::cout << "--- DEPLOYING SQUAD ---\n\n";
	std::shared_ptr<MissionData> sharedIntl = std::make_shared<MissionData>("Infiltrate the compound");

	Soldier alpha("Alpha", sharedIntl);
	Soldier bravo("Bravo", sharedIntl);
	Soldier charlie("Charlie", sharedIntl);

	std::cout << "\n *** INITIAL INTEL CHECK *** \n";
	alpha.readIntel();
	bravo.readIntel();
    charlie.readIntel();

    bravo.updateIntel("This is a trap!!!! Abort!!!");
    std::cout << "\n--- POST-UPDATE INTEL CHECK ---\n";
    alpha.readIntel();
    charlie.readIntel();

    std::cout << "\n--- MISSION OVER ---\n";

    return 0;
}