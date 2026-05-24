#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstdio>

auto fileDeleter = [](FILE *file)
{
    if (file)
    {
        std::cout << "   [RAII] Safely closing OS file handle...\n";
        std::fclose(file);
    }
};

using SafeCFile = std::unique_ptr<FILE, decltype(fileDeleter)>;

class FastBinaryWriter
{
private:
    SafeCFile file;

public:
    FastBinaryWriter(const std::string &filename) : file(nullptr, fileDeleter)
    {
        FILE *rawFile = std::fopen(filename.c_str(), "wb");
        if (!rawFile)
        {
            throw std::runtime_error("Could not open file for writing.");
        }
        file.reset(rawFile);
    }

    template <typename T>
    void writePrimative(const T &data)
    {
        std::fwrite(&data, sizeof(T), 1, file.get());
    }

    void writeString(const std::string &str)
    {
        size_t length = str.size();
        writePrimative(length);
        if (length > 0)
        {
            std::fwrite(str.data(), sizeof(char), length, file.get());
        }
    }

    template <typename T>
    void writeVector(const std::vector<T> &vec)
    {
        size_t length = vec.size();
        writePrimative(length);
        if (length > 0)
        {
            std::fwrite(vec.data(), sizeof(T), length, file.get());
        }
    }

    template <typename T>
    void writeComplexVector(const std::vector<T> &vec)
    {
        size_t length = vec.size();
        writePrimative(length);
        for (const auto &item : vec)
        {
            item.serialize(*this);
        }
    }
};

class FastBinaryReader
{
private:
    SafeCFile file;

public:
    FastBinaryReader(const std::string &filename) : file(nullptr, fileDeleter)
    {
        FILE *rawFile = std::fopen(filename.c_str(), "rb");
        if (!rawFile)
        {
            throw std::runtime_error("Could not open file for reading.");
        }
        file = SafeCFile(rawFile, fileDeleter);
    }

    template <typename T>
    void readPrimative(T &data)
    {
        std::fread(&data, sizeof(T), 1, file.get());
    }

    void readString(std::string &str)
    {
        size_t length = 0;
        readPrimative(length);
        str.resize(length);
        if (length > 0)
        {
            std::fread(&str[0], sizeof(char), length, file.get());
        }
    }

    template <typename T>
    void readVector(std::vector<T> &vec)
    {
        size_t length = 0;
        readPrimative(length);
        vec.resize(length);
        if (length > 0)
        {
            std::fread(vec.data(), sizeof(T), length, file.get());
        }
    }

    template <typename T>
    void readComplexVector(std::vector<T> &vec)
    {
        size_t length = 0;
        readPrimative(length);
        vec.resize(length);
        for (int i = 0; i < length; i++)
        {
            vec[i].deserialize(*this);
        }
    }
};

struct Item
{
    std::string name;
    int durability;

    void serialize(FastBinaryWriter &writer) const
    {
        writer.writeString(name);
        writer.writePrimative(durability);
    }

    void deserialize(FastBinaryReader &reader)
    {
        reader.readString(name);
        reader.readPrimative(durability);
    }
};

struct PlayerData
{
    std::string playerName;
    int level;
    std::vector<int> stats;
    std::vector<Item> inventory;

    void serialize(FastBinaryWriter &writer) const
    {
        writer.writeString(playerName);
        writer.writePrimative(level);
        writer.writeVector(stats);
        writer.writeComplexVector(inventory);
    }

    void deserialize(FastBinaryReader &reader)
    {
        reader.readString(playerName);
        reader.readPrimative(level);
        reader.readVector(stats);
        reader.readComplexVector(inventory);
    }
};

int main()
{
    std::string saveFile = "hybrid_savegame.bin";

    std::cout << "--- STEP 1: SAVING ---\n";
    try
    {
        PlayerData p1;
        p1.playerName = "Hybrid_Master";
        p1.level = 99;
        p1.stats = std::vector<int>(50, 42); // 50 stats, all set to 42
        p1.inventory.push_back({"Obsidian Sword", 200});
        p1.inventory.push_back({"Mana Potion", 5});

        // The FastBinaryWriter is created inside this block
        FastBinaryWriter writer(saveFile);
        p1.serialize(writer);

        std::cout << "[+] Data serialized successfully using fast C-streams!\n";

        // As soon as this block ends, 'writer' goes out of scope.
        // It will automatically trigger the RAII fileDeleter!
    }
    catch (const std::exception &e)
    {
        std::cerr << "Save Error: " << e.what() << '\n';
    }

    std::cout << "\n--- STEP 2: LOADING ---\n";
    try
    {
        PlayerData p2;

        FastBinaryReader reader(saveFile);
        p2.deserialize(reader);
        std::cout << "[+] Data deserialized successfully!\n";

        std::cout << "\n--- VERIFYING LOADED DATA ---\n";
        std::cout << "Player: " << p2.playerName << " (Level " << p2.level << ")\n";
        std::cout << "Stats Count: " << p2.stats.size() << " (Stat #25 is " << p2.stats[25] << ")\n";
        std::cout << "Inventory (" << p2.inventory.size() << " items):\n";
        for (const auto &item : p2.inventory)
        {
            std::cout << "  - " << item.name << " (Durability: " << item.durability << ")\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Load Error: " << e.what() << '\n';
    }
    return 0;
}