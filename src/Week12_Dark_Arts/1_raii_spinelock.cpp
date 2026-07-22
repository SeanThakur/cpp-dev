#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>

class Spinelock
{
private:
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    void lock()
    {
        while (locked.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    void unlock()
    {
        locked.clear(std::memory_order_release);
    }
};

Spinelock engineLock;
int counter = 0;

void renderFrame(int threadId)
{
    try
    {
        std::lock_guard<Spinelock> lock(engineLock);
        counter++;
        if (counter == 5)
        {
            throw std::runtime_error("GPU Device Lost!");
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error : " << e.what() << "\n";
    }
}

int main()
{
    std::cout << "--- MONDAY PART 2: THE RAII SPINLOCK ---\n\n";
    std::vector<std::thread> poll;

    for (int i = 0; i < 10; i++)
    {
        poll.emplace_back(std::thread(renderFrame, i));
    }

    for (auto &t : poll)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    std::cout << "\n[Engine] Recovered. Final Frame Count: " << counter << "\n";
    return 0;
}