#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
#include <cstring>
#include <stdexcept>

class Texture2D
{
private:
    std::string name;
    size_t size;
    int *pixels;

public:
    Texture2D(const std::string &name, size_t size) : name(name), size(size)
    {
        pixels = new int[size];
        std::cout << "Texture2D " << name << " created with size " << size << "." << std::endl;
    }

    Texture2D(const Texture2D &other) = delete;
    Texture2D &operator=(const Texture2D &other) = delete;

    Texture2D(Texture2D &&other) noexcept
    {
        name = std::move(other.name);
        size = other.size;
        pixels = other.pixels;
        other.pixels = nullptr; // Prevent double deletion
        std::cout << "Texture2D " << name << " moved." << std::endl;
    }

    ~Texture2D()
    {
        delete[] pixels;
        std::cout << "Texture2D " << name << " destroyed." << std::endl;
    }

    std::string getName() const
    {
        return name;
    }
};

auto fileDeleter = [](FILE *file)
{
    if (file)
    {
        std::fclose(file);
        std::cout << "File closed." << std::endl;
    }
};

class GameConfig
{
private:
    std::string filename;
    std::unique_ptr<FILE, decltype(fileDeleter)> osTicket;

public:
    GameConfig(const std::string &name) : filename(name), osTicket(nullptr, fileDeleter)
    {
        FILE *rawFile = std::fopen(filename.c_str(), "w");
        if (!rawFile)
        {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        osTicket.reset(rawFile);
        std::cout << "[+] OS Boundary: Acquired Ticket for '" << filename << "'.\n";
    }

    std::string getName() const
    {
        return filename;
    }
};

template <typename T>
class ResourceManager
{
private:
    std::vector<std::unique_ptr<T>> pool;

public:
    ResourceManager(const ResourceManager &other) = delete;            // Disable copy constructor
    ResourceManager &operator=(const ResourceManager &other) = delete; // Disable copy assignment operator
    ResourceManager() = default;

    void loadAsset(std::unique_ptr<T> resource)
    {
        std::cout << "    [Manager] Registering " << resource->getName() << " into master pool.\n";
        pool.push_back(std::move(resource));
    }

    T *getAsset(const std::string &assetName)
    {
        for (const auto &ptr : pool)
        {
            if (ptr->getName() == assetName)
            {
                return ptr.get();
            }
        }
        std::cerr << "[!] Engine Warning: Asset '" << assetName << "' not found in pool." << std::endl;
        return nullptr;
    }

    void flushMemory()
    {
        std::cout << "\n[Manager] FLUSHING ALL ASSETS FROM RAM...\n";
        pool.clear();
    }
};

int main()
{
    std::cout << "--- THE UNIFIED ASSET MANAGER ---\n\n";
    try
    {
        ResourceManager<Texture2D> textureManager;
        ResourceManager<GameConfig> configManager;
        std::cout << ">> PHASE 1: LOADING ASSETS\n";
        auto textureName1 = std::make_unique<Texture2D>("HeroSprite", 1024);
        auto textureName2 = std::make_unique<Texture2D>("LevelMap", 8192);
        auto configName1 = std::make_unique<GameConfig>("video_settings.ini");
        auto configName2 = std::make_unique<GameConfig>("audio_settings.ini");

        textureManager.loadAsset(std::move(textureName1));
        textureManager.loadAsset(std::move(textureName2));
        configManager.loadAsset(std::move(configName1));
        configManager.loadAsset(std::move(configName2));
        std::cout << "\n>> PHASE 2: GAME LOOP EXECUTION\n";

        Texture2D *hero = textureManager.getAsset("HeroSprite");
        if (hero)
        {
            std::cout << "    [GameLoop] Successfully bound '" << hero->getName() << "' to the renderer.\n";
        }
        std::cout << "\n>> PHASE 3: LEVEL COMPLETE. TRIGGERING FLUSH.\n";
        textureManager.flushMemory();
        configManager.flushMemory();
    }
    catch (const std::exception &e)
    {
        std::cout << "\n[!] FATAL ENGINE CRASH AVERTED: " << e.what() << "\n";
    }
    std::cout << "\n--- MAIN THREAD TERMINATED SAFELY ---\n";
    return 0;
}