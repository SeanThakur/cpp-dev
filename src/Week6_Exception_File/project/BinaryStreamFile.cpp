#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

class BinaryWriter
{
private:
    std::ofstream file;

public:
    BinaryWriter(const std::string &filename)
    {
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file for writing.");
        }
    }

    template <typename T>
    void writePrimitive(const T &data)
    {
        file.write(reinterpret_cast<const char *>(&data), sizeof(T));
    }

    void writePrimitiveString(const std::string &str)
    {
        size_t length = str.size();
        writePrimitive(length);
        if (length > 0)
        {
            file.write(str.data(), length);
        }
    }

    template <typename T>
    void writePrimitiveVector(const std::vector<T> &item)
    {
        size_t length = item.size();
        writePrimitive(length);
        if (length > 0)
        {
            file.write(reinterpret_cast<const char *>(item.data()), length * sizeof(T));
        }
    }

    template <typename T>
    void writeComplexVector(const std::vector<T> &vec)
    {
        size_t length = vec.size();
        writePrimitive(length);
        for (const auto &item : vec)
        {
            item.serialize(*this);
        }
    }
};

class BinaryReader
{
private:
    std::ifstream file;

public:
    BinaryReader(const std::string &filename)
    {
        file.open(filename, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file for reading.");
        }
    }

    template <typename T>
    void readPrimitive(T &data)
    {
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
    }

    void readPrimitiveString(std::string &str)
    {
        size_t length = 0;
        readPrimitive(length);
        str.resize(length);
        if (length > 0)
        {
            file.read(&str[0], length);
        }
    }

    template <typename T>
    void readPrimitiveVector(std::vector<T> &vec)
    {
        size_t length = 0;
        readPrimitive(length);
        vec.resize(length);
        if (length > 0)
        {
            file.read(reinterpret_cast<char *>(vec.data()), length * sizeof(T));
        }
    }

    template <typename T>
    void readComplexVector(std::vector<T> &vec)
    {
        size_t length = 0;
        readPrimitive(length);
        vec.resize(length);
        for (size_t i = 0; i < length; i++)
        {
            vec[i].deserialize(*this);
        }
    }
};

struct Item
{
    std::string name;
    int durability;

    void serialize(BinaryWriter &writer) const
    {
        writer.writePrimitive(durability);
        writer.writePrimitiveString(name);
    }

    void deserialize(BinaryReader &reader)
    {
        reader.readPrimitive(durability);
        reader.readPrimitiveString(name);
    }
};

struct PlayerData
{
    std::string playerName;
    int level;
    std::vector<int> stats;
    std::vector<Item> inventory;

    void serialize(BinaryWriter &writer) const
    {
        writer.writePrimitiveString(playerName);
        writer.writePrimitive(level);
        writer.writePrimitiveVector(stats);
        writer.writeComplexVector(inventory);
    }

    void deserialize(BinaryReader &reader)
    {
        reader.readPrimitiveString(playerName);
        reader.readPrimitive(level);
        reader.readPrimitiveVector(stats);
        reader.readComplexVector(inventory);
    }
};

int main()
{
    std::string saveFile = "epic_savegame.bin";

    // --- STEP 1: CREATE AND SAVE ---
    try
    {
        PlayerData p1;
        p1.playerName = "Sean_The_Architect";
        p1.level = 42;

        // Add 200 dummy stats (let's make them all equal 99)
        p1.stats = std::vector<int>(200, 99);

        // Add complex items
        p1.inventory.push_back({"Steel Sword", 100});
        p1.inventory.push_back({"Health Potion", 1});
        p1.inventory.push_back({"Dragon Shield", 500});

        std::cout << ">> Writing to Hard Drive...\n";
        BinaryWriter writer(saveFile);
        p1.serialize(writer);
        std::cout << "[+] Save Complete.\n\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Save Error: " << e.what() << '\n';
        return 1;
    }

    // --- STEP 2: LOAD AND VERIFY ---
    try
    {
        PlayerData p2; // Totally blank player

        std::cout << ">> Reading from Hard Drive...\n";
        BinaryReader reader(saveFile);
        p2.deserialize(reader);

        std::cout << "[+] Load Complete.\n\n";

        // Verify Data
        std::cout << "--- LOADED DATA VERIFICATION ---\n";
        std::cout << "Player: " << p2.playerName << " (Level " << p2.level << ")\n";
        std::cout << "Stats Count: " << p2.stats.size() << " (Stat #150 is " << p2.stats[150] << ")\n";

        std::cout << "Inventory (" << p2.inventory.size() << " items):\n";
        for (const auto &item : p2.inventory)
        {
            std::cout << "  - " << item.name << " (Durability: " << item.durability << ")\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Load Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}