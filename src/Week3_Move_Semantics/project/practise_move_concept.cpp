#include <iostream>

void processData(const std::string &data)
{
    std::cout << "Processing L Value data: " << data << std::endl;
}

void processData(std::string &&data)
{
    std::cout << "Processing R Value data: " << data << std::endl;
}

int main()
{
    std::string myData = "Hello, World!";
    processData(myData);            // L value
    processData(std::move(myData)); // R value
    processData("TemporaryString"); // R value (string literal)
    return 0;
}