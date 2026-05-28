#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>

void downloadFile(const std::string &fileName, std::function<void(std::string)> onSuccess)
{
    std::cout << "[Network] Initiating connection to server...\n";
    std::cout << "[Network] Downloading '" << fileName << "'...\n";

    for (int i = 0; i < 100; i += 25)
    {
        std::cout << "-> Progress:" << i << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    std::cout << "[Network] Download Complete!\n";

    std::string downloadedPayload = fileName + " __BINARY DATA__";
    onSuccess(downloadedPayload);
}

int main()
{
    std::cout << "--- CALLBACKS & ASYNC MINDSET ---\n\n";

    std::cout << ">> Requesting Level 1 Assets from server...\n\n";

    downloadFile("downoad_simulator.apk", [](std::string downloadedPayload)
                 {
        std::cout << "    [Game Engine] Callback Received!\n";
        std::cout << "    [Game Engine] Extracting payload: " << downloadedPayload << "\n";
        std::cout << "    [Game Engine] Loading level into RAM... Success.\n"; });

    std::cout << "\n--- MAIN THREAD FINISHED ---\n";
    return 0;
}