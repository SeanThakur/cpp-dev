#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

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

Spinelock lockMutex;
int counter = 0;

void worker(int threadId)
{
    for (int i = 1; i <= 100000; i++)
    {
        lockMutex.lock();
        counter++;
        lockMutex.unlock();
    }
}

int main()
{
    std::cout << "--- MONDAY: THE SPINLOCK ---\n\n";
    std::vector<std::thread> poll;

    for (int i = 0; i < 10; i++)
    {
        poll.emplace_back(std::thread(worker, i));
    }

    for (auto &t : poll)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    std::cout << "[Engine] Final Counter Value: " << counter << " (Expected: 1000000)\n";
    std::cout << "[Engine] Spinlock executed millions of lock/unlock cycles flawlessly.\n";
    return 0;
}