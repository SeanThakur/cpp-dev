#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Inventory
{
    std::string name;
    Inventory(std::string n) : name(n)
    {
        std::cout << "--> Inventory " << name << " Created \n";
    }
};

class Guild;

class Player
{
public:
    std::string name;
    std::unique_ptr<Inventory> bag;
    std::shared_ptr<Guild> myGuild;

    Player(std::string n) : name(n)
    {
        std::cout << "--> Player " << name << " Created \n";
    }
};

class Guild
{
public:
    std::string name;
    std::vector<std::weak_ptr<Player>> onlineMembers;

    Guild(std::string n) : name(n)
    {
        std::cout << "--> Guild " << name << " Created \n";
    }

    void broadcastMessage()
    {
        for (auto &member : onlineMembers)
        {
            if (std::shared_ptr<Player> tempMember = member.lock())
            {
                std::cout << "Member " << tempMember->name << " Recived the Message!! \n";
            }
            else
            {
                std::cout << "Member is vanised \n";
            }
        }
    }
};

int main()
{
    std::cout << "\n*** MAIN Program ***\n";
    std::shared_ptr<Guild> room = std::make_shared<Guild>("Sabhya samaj");
    {
        std::shared_ptr<Player> p1 = std::make_shared<Player>("Sean");
        std::shared_ptr<Player> p2 = std::make_shared<Player>("Vikas");

        p1->myGuild = room;
        p2->myGuild = room;

        room->onlineMembers.push_back(p1);
        room->onlineMembers.push_back(p2);

        room->broadcastMessage();

        p2.reset();

        room->broadcastMessage();
    }

    room->broadcastMessage();
    std::cout << "\n*** END Program ***\n";
    return 0;
}