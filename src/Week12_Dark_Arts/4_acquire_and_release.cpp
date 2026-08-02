#include <iostream>
#include <thread>
#include <atomic>
#include <string>

std::string sharedPayload = "";
std::atomic<bool> payloadReady{false};

void NetworkProducer()
{
    sharedPayload = "Player 1 is done bro!!";
    payloadReady.store(true, std::memory_order_release);
    std::cout << "[Network] Payload downloaded and Release Fence raised.\n";
}

void PhysicsConsumer()
{
    while (!payloadReady.load(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }
    std::cout << "[Physics] Acquired payload: " << sharedPayload << "\n";
}

int main()
{
    std::cout << "--- THURSDAY PART 2: ACQUIRE / RELEASE ---\n\n";
    std::thread t1(NetworkProducer);
    std::thread t2(PhysicsConsumer);
    std::thread t3(NetworkProducer);
    std::thread t4(PhysicsConsumer);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}