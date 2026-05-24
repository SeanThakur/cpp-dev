#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

struct GameSetting
{
    int resolution;
    int volumn;
};

GameSetting loadConfig(const std::string &filename)
{
    GameSetting setting;
    std::ifstream config(filename);
    if (!config.is_open())
    {
        throw std::runtime_error("File '" + filename + "' could not be opened or found.");
    }

    std::string line;
    while (std::getline(config, line))
    {
        if (line.empty())
        {
            continue;
        }

        size_t equalPos = line.find("=");
        if (equalPos == std::string::npos)
        {
            throw std::invalid_argument("Malformed line (missing '='): " + line);
        }

        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);

        if (key == "Resolution")
        {
            setting.resolution = std::stoi(value);
        }
        else if (key == "Volume")
        {
            int vol = std::stoi(value);
            if (vol < 0 || vol > 100)
            {
                throw std::out_of_range("Volume must be between 0 and 100. Received: " + value);
            }
            setting.volumn = vol;
        }
    }
    return setting;
}

int main()
{
    GameSetting currentSettings = {1080, 50};
    try
    {
        std::cout << "[+] Attempting to load 'settings.txt'...\n";

        // If this succeeds, it overwrites our defaults.
        // If it throws an error, the code immediately jumps out of the try block!
        currentSettings = loadConfig("settings.txt");

        std::cout << "[+] Settings loaded successfully!\n";
    }
    // We can catch different errors in different ways!
    catch (const std::runtime_error &e)
    {
        std::cerr << "[-] SYSTEM ERROR: " << e.what() << "\n";
        std::cerr << "[-] Action: Using default settings.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "[-] PARSE ERROR: " << e.what() << "\n";
        std::cerr << "[-] Action: File is corrupted. Using default settings.\n";
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "[-] LOGIC ERROR: " << e.what() << "\n";
        std::cerr << "[-] Action: Values unsafe. Using default settings.\n";
    }
    // A catch-all for any other std::exception we might have missed (like std::stoi failing)
    catch (const std::exception &e)
    {
        std::cerr << "[-] UNKNOWN ERROR: " << e.what() << "\n";
    }

    // Prove that the game can still safely run regardless of what happened above
    std::cout << "\n--- GAME STARTING WITH --- \n";
    std::cout << "Resolution: " << currentSettings.resolution << "\n";
    std::cout << "Volume: " << currentSettings.volumn << "\n";
    return 0;
}