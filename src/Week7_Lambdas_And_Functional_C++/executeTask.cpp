#include <iostream>
#include <functional>

void executeTask(std::function<void()> task)
{
    std::cout << "[Engine] Task received. Preparing execution environment...\n";
    task();
    std::cout << "[Engine] Task execution complete.\n";
}

int main()
{
    std::cout << "--- std::function (LOGIC AS DATA) ---\n\n";
    auto printTask = []()
    {
        std::cout << "    -> [Task] Printing critical system data to screen...\n";
    };
    auto mathTask = []()
    {
        std::cout << "    -> [Task] Printing critical Math data to screen...\n";
        return 30 * 100;
    };

    std::cout << ">> Sending Print Task to Engine...\n";
    executeTask(printTask);

    std::cout << "\n>> Sending Math Task to Engine...\n";
    executeTask(mathTask);

    std::cout << "\n>> Sending Inline Task to Engine...\n";
    executeTask([]()
                { std::cout << "    -> [Inline Task] Printing Inline task to screen...\n"; });
    return 0;
}