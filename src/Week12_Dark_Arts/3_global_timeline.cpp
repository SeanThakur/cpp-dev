#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> x{false};
std::atomic<bool> y{false};
std::atomic<int> z{0};

void writeX()
{
    x.store(true, std::memory_order_seq_cst);
}

void writeY()
{
    y.store(true, std::memory_order_seq_cst);
}

void observer1()
{
    // wait until x has happened
    while (!x.load(std::memory_order_seq_cst))
    {
        std::this_thread::yield();
    }
    // Now that X happened, check if Y has ALSO happened.
    if (y.load(std::memory_order_seq_cst))
    {
        z++;
    }
}

void observer2()
{
    // wait until y has happened
    while (!y.load(std::memory_order_seq_cst))
    {
        std::this_thread::yield();
    }

    if (x.load(std::memory_order_seq_cst))
    {
        z++;
    }
}

int main()
{
    std::cout << "--- WEDNESDAY PART 2: THE GLOBAL TIMELINE ---\n\n";
    std::thread a(writeX);
    std::thread b(writeY);
    std::thread c(observer1);
    std::thread d(observer2);

    a.join();
    b.join();
    c.join();
    d.join();

    std::cout << "[Engine] sharedZ value is: " << z.load() << "\n";
    std::cout << "[Engine] Because of seq_cst, sharedZ can NEVER be 0.\n";
    return 0;
}