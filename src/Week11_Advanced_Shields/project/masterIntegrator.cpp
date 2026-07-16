#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <mutex>
#include <chrono>
#include <functional>

std::shared_mutex ecoMutex;
float tax = 0.10f;

struct Player
{
    std::string name;
    int gold = 100;
    std::mutex pMutex;

    Player(std::string n, int g) : name(n), gold(g) {}
};

void aiPriceChecker(int aiId)
{
    std::shared_lock<std::shared_mutex> lock(ecoMutex);
    std::cout << "[AI " << aiId << "] Reading global tax rate: " << (tax * 100) << "%\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void adminUpdateTax()
{
    std::cout << "\n[Admin] Attempting to update global economy...\n";
    std::unique_lock<std::shared_mutex> writerLock(ecoMutex, std::try_to_lock);
    if (writerLock.owns_lock())
    {
        tax = 0.15f;
        std::cout << "[Admin] SUCCESS! Global tax raised to 15%.\n\n";
    }
    else
    {
        std::cout << "[Admin] FAILED! Economy system busy. Skipping frame.\n\n";
    }
}

void executeTrade(Player &player1, Player &player2, int tradeId, int amount)
{
    std::scoped_lock dualLock(player1.pMutex, player2.pMutex);
    std::cout << "[Trade " << tradeId << "] Secured " << player1.name << " and " << player2.name << ".\n";
    float currentTax = 0.0f;
    // reading the latest tax
    {
        std::shared_lock<std::shared_mutex> ecoLock(ecoMutex);
        currentTax = tax;
    }
    int taxAmount = amount * currentTax;
    int finalTransfer = amount - taxAmount;
    if (player1.gold >= amount)
    {
        player1.gold -= amount;
        player2.gold += finalTransfer;
        std::cout << "  -> [Trade " << tradeId << "] SUCCESS. " << player1.name << " paid " << amount << "g (" << taxAmount << "g tax). " << player2.name << " received " << finalTransfer << "g.\n";
    }
}

int main()
{
    std::cout << "--- WEEK 11 FINAL BOSS: THE MASTER INTEGRATOR ---\n\n";
    Player sean("Sean", 1000);
    Player vikas("Vikas", 800);
    std::vector<std::thread> engineThreads;

    // 1. Spawn the AI Reader Horde
    for (int i = 1; i <= 5; i++)
    {
        engineThreads.emplace_back(std::thread(aiPriceChecker, i));
    }

    // 2. Spawn two simultaneous conflicting trades
    // Sean sends to Vikas, while Vikas sends to Sean
    engineThreads.emplace_back(std::thread(executeTrade, std::ref(sean), std::ref(vikas), 1, 100));
    engineThreads.emplace_back(std::thread(executeTrade, std::ref(vikas), std::ref(sean), 2, 100));

    // 3. Admin tries to interrupt and change the tax rate
    engineThreads.emplace_back(std::thread(adminUpdateTax));

    // 4. Wait for all simulation threads to finish
    for (auto &t : engineThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    std::cout << "\n[Engine] Simulation Complete.\n";
    std::cout << "Final Balances -> Sean: " << sean.gold << "g, Vikas: " << vikas.gold << "g\n";
    return 0;
}