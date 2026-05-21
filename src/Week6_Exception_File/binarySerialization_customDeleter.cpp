#include <iostream>
#include <memory>
#include <cstring>
#include <cstdio>

class MyString
{
public:
    int length;
    char *data;

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
        if (this != &other)
        {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            std::strcpy(data, other.data);
        }

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
    int level;
    MyString name;
};

auto fileDeleter = [](FILE *file)
{
    if (file != nullptr)
    {
        std::cout << "[-] Demolition Crew: Safely executing fclose() on OS Ticket.\n";
        std::fclose(file);
    }
};

using safeFile = std::unique_ptr<FILE, decltype(fileDeleter)>;

void saveGame(const PlayerData &player)
{
    std::cout << "\n>> SAVING GAME...\n";
    FILE *rawFile = std::fopen("saveGame.bin", "wb");
    if (!rawFile)
    {
        return;
    }

    safeFile saveFile(rawFile, fileDeleter);

    std::fwrite(&player.level, sizeof(int), 1, saveFile.get());
    std::fwrite(&player.name.length, sizeof(int), 1, saveFile.get());
    std::fwrite(player.name.data, sizeof(char), player.name.length, saveFile.get());
    std::cout << "[+] Saved Player: " << player.name.data << " (Level " << player.level << ")\n";
}

void loadGame(PlayerData &player)
{
    std::cout << "\n>> LOADING GAME...\n";
    FILE *rawFile = std::fopen("saveGame.bin", "rb");
    if (!rawFile)
    {
        return;
    }

    safeFile loadFile(rawFile, fileDeleter);

    std::fread(&player.level, sizeof(int), 1, loadFile.get());
    int incomingLength = 0;
    std::fread(&incomingLength, sizeof(int), 1, loadFile.get());
    player.name.allocateMemory(incomingLength);
    std::fread(player.name.data, sizeof(char), incomingLength, loadFile.get());
    std::cout << "[+] Loaded Player: " << player.name.data << " (Level " << player.level << ")\n";
}

int main()
{
    std::cout << "--- BIG EXAMPLE: BINARY SERIALIZATION + CUSTOM DELETERS ---\n";
    PlayerData player;
    player.name = "Invoker";
    player.level = 25;

    saveGame(player);

    PlayerData player2;
    player2.name = "Placeholder";
    loadGame(player2);
    return 0;
}