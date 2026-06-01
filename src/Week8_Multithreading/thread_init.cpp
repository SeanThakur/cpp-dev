#include <iostream>
#include <thread>
#include <vector>

int main()
{
    std::cout << "--- MONDAY: SPINNING UP THE CORE ---\n\n";
    std::cout << "[Main] Booting up System...\n";
    std::vector<std::thread> poll;
    unsigned int count = std::thread::hardware_concurrency(); // ask OS to give available threads
    if (count == 0)
    {
        count = 2; // if no threads are available then allote 2 of them to work
    }

    for (int i = 0; i < count; i++)
    {
        poll.push_back(std::thread([=]()
                                   { std::cout << "   -> Hello from Background Thread #" << i << "!\n"; }));
    }

    std::cout << "[Main] All workers dispatched. Waiting for them to finish...\n";

    for (auto &p : poll)
    {
        if (p.joinable())
        {
            p.join();
        }
    }

    std::cout << "\n[Main] All workers successfully joined. Shutting down safely.\n";
    return 0;
}