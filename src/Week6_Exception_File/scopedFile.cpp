#include <iostream>
#include <cstdio>
#include <utility>
#include <string>

std::string BASE_FILE_DIR = "/Users/seanthakur/Documents/cpp-practice/src/Week6_Exception_File/utils/";

class ScopedFile
{
private:
    FILE *fileHandle;

public:
    ScopedFile(const char *filename, const char *mode)
    {
        std::string filePath = BASE_FILE_DIR + filename;
        fileHandle = std::fopen(filePath.c_str(), mode);
        if (fileHandle != nullptr)
        {
            std::cout << "[+] File open \n";
        }
        else
        {
            std::cout << "[!] ERROR while opening a file \n";
        }
    }

    ~ScopedFile()
    {
        if (fileHandle != nullptr)
        {
            std::fclose(fileHandle);
            std::cout << "[-] File is destroyed successfully \n";
        }
    }

    // Delete copy constructor and copy assignment operator to prevent copying of the resource
    ScopedFile(const ScopedFile &other) = delete;
    ScopedFile &operator=(const ScopedFile &other) = delete;

    ScopedFile(ScopedFile &&other) noexcept
    {
        std::cout << "[+] Move Constructor \n";
        fileHandle = other.fileHandle;
        other.fileHandle = nullptr;
    }

    ScopedFile &operator=(ScopedFile &&other) noexcept
    {
        std::cout << "[+] Move Assignment Constructor \n";
        if (this == &other)
        {
            return *this;
        }

        if (fileHandle != nullptr)
        {
            std::fclose(fileHandle);
        }
        fileHandle = other.fileHandle;
        other.fileHandle = nullptr;
        return *this;
    }

    void writeLine(const char *payload)
    {
        if (fileHandle != nullptr)
        {
            std::fprintf(fileHandle, "%s\n", payload);
        }
    }
};

ScopedFile openGameConfig()
{
    std::cout << ">> Factory: Opening config file...\n";
    ScopedFile gamefile("config.ini", "w");
    gamefile.writeLine("Resolution=1920x1080");
    return gamefile;
}

int main()
{
    std::cout << "--- HARDWARE OWNERSHIP TRANSFER ---\n\n";
    // Create an artificial scope using curly braces
    {
        std::cout << ">> Entering internal scope...\n";

        // We ask the OS for a ticket to "player_data.txt"
        ScopedFile saveFile("player_data.txt", "w");

        std::cout << ">> Writing data to disk...\n";
        saveFile.writeLine("PlayerName: Sean");
        saveFile.writeLine("Level: 99");
        saveFile.writeLine("Gold: 5000");

        std::cout << ">> Leaving internal scope...\n";
    } // The moment we hit this bracket, saveFile is destroyed!

    // 1. We receive the file ticket via Move Semantics. Zero copies!
    ScopedFile myConfig = openGameConfig();

    // 2. We can still write to it because we now own the ticket.
    myConfig.writeLine("VSync=True");

    std::cout << ">> main() is ending. Watch the Destructor fire safely!\n";
    return 0;
}