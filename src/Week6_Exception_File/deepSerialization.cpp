#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

std::string BASE_FILE_DIR = "/Users/seanthakur/Documents/cpp-practice/src/Week6_Exception_File/utils/";

class MyString
{
public:
    char *data;
    int length;

    MyString(const char *str = "")
    {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
    }

    ~MyString()
    {
        if (data != nullptr)
        {
            delete[] data;
        }
    }

    MyString(const MyString &other)
    {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }

    MyString &operator=(const MyString &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (data != nullptr)
        {
            delete[] data;
        }

        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);

        return *this;
    }

    void allocateMemory(int newLength)
    {
        delete[] data;
        length = newLength;
        data = new char[length + 1];
        data[length] = '\0';
    }
};

struct PlayerData
{
    MyString name;
    int level;
};

void saveGame(const PlayerData &player)
{
    std::string fullPath = BASE_FILE_DIR + "dotaGame.bin";
    std::ofstream saveFile(fullPath.c_str(), std::ios::out | std::ios::binary);
    if (!saveFile.is_open())
    {
        std::cerr << "[-] Error: Could not open save file to save.\n";
        return;
    }

    // save plain old data
    saveFile.write(reinterpret_cast<const char *>(&player.level), sizeof(int));

    // DEEP SERIALIZATION: Save the String
    saveFile.write(reinterpret_cast<const char *>(&player.name.length), sizeof(int));
    saveFile.write(player.name.data, player.name.length);

    std::cout << "[+] Game saved with Player " << player.name.data << "is saved with level " << player.level << "\n";
}

void loadGame(PlayerData &player)
{
    std::string fullPath = BASE_FILE_DIR + "dotaGame.bin";
    std::ifstream readSavedFile(fullPath.c_str(), std::ios::in | std::ios::binary);
    if (!readSavedFile.is_open())
    {
        std::cerr << "[-] Error: Could not open save file to load.\n";
        return;
    }

    // Load Plain Old Data directly
    readSavedFile.read(reinterpret_cast<char *>(&player.level), sizeof(int));

    // DEEP DESERIALIZATION: Load the String
    int incomingNameLen = 0;
    readSavedFile.read(reinterpret_cast<char *>(&incomingNameLen), sizeof(int));
    player.name.allocateMemory(incomingNameLen);
    readSavedFile.read(player.name.data, incomingNameLen);

    std::cout << "[+] Loaded Player: " << player.name.data << "\n";
}

int main()
{
    std::cout << "--- DEEP BINARY SERIALIZATION TEST ---\n\n";

    // 1. Create a player and give them deep memory data
    PlayerData p1;
    p1.name = MyString("Sean_The_Architect");
    p1.level = 99;

    std::cout << ">> Dumping RAM to Hard Drive...\n";
    saveGame(p1);

    // 2. Create a totally blank player (Memory currently holds "")
    PlayerData p2;

    std::cout << "\n>> Injecting Hard Drive data into blank RAM...\n";
    loadGame(p2);
    return 0;
}
