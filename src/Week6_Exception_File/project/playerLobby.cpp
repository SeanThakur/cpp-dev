// With Custom string class
#include <iostream>
#include <cstdio>
#include <memory>
#include <cstring>
#include <stdexcept>

class DeepString
{
public:
    char *data;
    int length;

    DeepString(const char *name = "")
    {
        length = std::strlen(name);
        data = new char[length + 1];
        std::strcpy(data, name);
    }

    ~DeepString()
    {
        if (data != nullptr)
        {
            delete[] data;
        }
    }

    DeepString(const DeepString &other)
    {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }

    DeepString &operator=(const DeepString &other)
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

    void allocate(int newLength)
    {
        delete[] data;
        length = newLength;
        data = new char[length + 1];
        data[length] = '\0';
    }
};

struct Vector3
{
    float x, y, z;
};

struct Player
{
    DeepString name;
    int level;
    Vector3 position = {0, 0, 0};
};

struct WorldState
{
    int maxPlayers;
    int currentActivePlayer;

    std::unique_ptr<Player[]> players;

    WorldState(int max) : maxPlayers(max), currentActivePlayer(0)
    {
        players = std::make_unique<Player[]>(max);
    }
};

auto fileDeleter = [](FILE *file)
{
    if (file)
    {
        std::cout << "   [!] RAII: Safely executing fclose() to return OS ticket.\n";
        std::fclose(file);
    }
};

using safeCFile = std::unique_ptr<FILE, decltype(fileDeleter)>;

void saveMultiplePlayerWorld(const WorldState &world, const char *filename)
{
    std::cout << "\n>>> INITIATING WORLD SAVE...\n";
    FILE *rawFile = std::fopen(filename, "wb");
    if (!rawFile)
    {
        throw std::runtime_error("CRITICAL: Failed to open file.");
    }

    safeCFile saveFile(rawFile, fileDeleter);

    // 1. Serialize Header
    std::fwrite(&world.currentActivePlayer, sizeof(int), 1, saveFile.get());
    std::fwrite(&world.maxPlayers, sizeof(int), 1, saveFile.get());

    // 2. Deep Serialize Players
    for (int i = 0; i < world.currentActivePlayer; i++)
    {
        const Player &p = world.players[i];
        std::fwrite(&p.name.length, sizeof(int), 1, saveFile.get());
        std::fwrite(p.name.data, sizeof(char), p.name.length, saveFile.get());
        std::fwrite(&p.level, sizeof(int), 1, saveFile.get());
        std::fwrite(&p.position, sizeof(int), 1, saveFile.get());

        std::cout << "  -> Saved Player: " << p.name.data << "\n";
    }
    std::cout << ">>> WORLD SAVED SUCCESSFULLY.\n";
}

void loadMultiplePlayerWorld(WorldState &world, const char *filename)
{
    std::cout << "\n>>> INITIATING WORLD LOAD...\n";
    FILE *rawFile = std::fopen(filename, "rb");
    if (!rawFile)
    {
        throw std::runtime_error("CRITICAL: Failed to locate save file.");
    }

    safeCFile readFile(rawFile, fileDeleter);

    std::fread(&world.currentActivePlayer, sizeof(int), 1, readFile.get());
    std::fread(&world.maxPlayers, sizeof(int), 1, readFile.get());

    // 🚀 We re-allocate the smart array to match the incoming hard drive data
    world.players = std::make_unique<Player[]>(world.maxPlayers);
    for (int i = 0; i < world.currentActivePlayer; i++)
    {
        Player &p = world.players[i];

        int incomingLength = 0;
        std::fread(&incomingLength, sizeof(int), 1, readFile.get());
        p.name.allocate(incomingLength);
        std::fread(p.name.data, sizeof(char), incomingLength, readFile.get());
        std::fread(&p.level, sizeof(int), 1, readFile.get());
        std::fread(&p.position, sizeof(int), 1, readFile.get());
        std::cout << "  -> Loaded Player: " << p.name.data << " (Level " << p.level << ")\n";
    }
    std::cout << ">>> WORLD LOADED SUCCESSFULLY.\n";
}

int main()
{
    std::cout << "=========== GAME ENGINE NORMAL EXECUTION ===========\n";
    try
    {
        WorldState serverRoom(10);
        serverRoom.currentActivePlayer = 2;
        serverRoom.players[0].name = "Sean_Archetecht";
        serverRoom.players[0].level = 99;
        serverRoom.players[1].name = DeepString("NoobMaster69");
        serverRoom.players[1].level = 5;

        saveMultiplePlayerWorld(serverRoom, "savegame.bin");
        WorldState clientWorld(0);
        loadMultiplePlayerWorld(clientWorld, "savegame.bin");
    }
    catch (const std::exception &e)
    {
        std::cout << "Engine Error: " << e.what() << "\n";
    }
    return 0;
}