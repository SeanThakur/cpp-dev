#include <iostream>
#include <memory>
#include <string>

class Mission
{
public:
    std::string objective;
    Mission(std::string obj) : objective(obj)
    {
        std::cout << "Mission Created: " << objective << "\n";
    }
};

void passByValue(std::shared_ptr<Mission> data)
{
    std::cout << "[By Value] Use Count inside function: " << data.use_count() << " \n";
}

void passByReference(const std::shared_ptr<Mission> &data) {
    std::cout << "[By Const Reference] Use Count inside function: " << data.use_count() << " \n";
}

int main() {
    std::cout << "--- STARTING BENCHMARK ---\n\n";
    std::shared_ptr<Mission> intel = std::make_shared<Mission>("Top Secret Intel");

    std::cout << "Base Use Count in main(): " << intel.use_count() << "\n\n";

    passByValue(intel);
    passByReference(intel);

    std::cout << "\nFinal Use Count in main(): " << intel.use_count() << "\n";
    std::cout << "--- BENCHMARK COMPLETE ---\n";
    return 0;
}