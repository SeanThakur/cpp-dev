#include <iostream>
#include <thread>
#include <mutex>
#include <utility>

std::mutex dbMutex;
std::string dbState = "CLEAN";

std::unique_lock<std::mutex> acquireAndPrepareDB()
{
    std::cout << "  [DB Manager] Authenticating and acquiring lock...\n";
    std::unique_lock<std::mutex> lock(dbMutex);
    std::cout << "  [DB Manager] Lock acquired. Database is secure.\n";
    std::cout << "  [DB Manager] Transferring physical lock ownership to Main Engine...\n";
    return std::move(lock);
}

int main()
{
    std::cout << "--- TUESDAY: TRANSFERRING LOCK OWNERSHIP ---\n\n";
    std::unique_lock<std::mutex> engineLock = acquireAndPrepareDB();
    if (engineLock.owns_lock())
    {
        std::cout << "[Engine] --- CRITICAL SECTION START ---\n";
        dbState = "ENGINE_STATE";
        std::cout << "[Engine] Updating database state to: " << dbState << "\n";
        std::cout << "[Engine] --- CRITICAL SECTION END ---\n";
    }
    std::cout << "\n[Engine] Function ending. engineLock destructor will now unlock dbMutex.\n";
    return 0;
}