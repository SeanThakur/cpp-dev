#include <iostream>
#include <functional>

std::function<int(int)> createBrokenMultiplier()
{
    int factory = 5;
    auto multipliedValue = [=](int input) mutable
    {
        factory++;
        return factory * input;
    };
    return multipliedValue;
}

int main()
{
    std::cout << "--- THE DANGLING CAPTURE TRAP ---\n\n";
    auto captureLambdas = createBrokenMultiplier();

    int result = captureLambdas(10);
    int result1 = captureLambdas(10);
    int result2 = captureLambdas(10);
    std::cout << "Result: " << result << "\n";
    std::cout << "Result 1: " << result1 << "\n";
    std::cout << "Result 2: " << result2 << "\n";
    return 0;
}