#include <iostream>
#include <utility>

class NetworkPacket
{
private:
    size_t byte;
    char *payload;

public:
    NetworkPacket(size_t size)
    {
        std::cout << "Constructor:: called" << "\n";
        byte = size;
        payload = new char[byte];
        for (size_t i = 0; i < byte; i++)
        {
            payload[i] = 'V';
        }
    }
    NetworkPacket(const NetworkPacket &source)
    {
        std::cout << "Copy Constructor called \n";
        byte = source.byte;
        payload = new char[byte];
        for (size_t i = 0; i < byte; i++)
        {
            payload[i] = source.payload[i];
        }
    }

    NetworkPacket &operator=(const NetworkPacket &source)
    {
        std::cout << "Copy assignment Constructor called \n";
        if (this == &source)
        {
            return *this;
        }
        if (payload != nullptr)
        {
            delete[] payload;
        }
        byte = source.byte;
        payload = new char[byte];
        for (size_t i = 0; i < byte; i++)
        {
            payload[i] = source.payload[i];
        }

        return *this;
    }

    NetworkPacket(NetworkPacket &&source) noexcept
    {
        std::cout << "Move Constructor called \n";
        this->byte = source.byte;
        this->payload = source.payload;

        source.payload = nullptr;
        source.byte = 0;
    }

    NetworkPacket &operator=(NetworkPacket &&source) noexcept
    {
        std::cout << "Move assignment Constructor called \n";
        if (this == &source)
        {
            return *this;
        }

        if (this->payload != nullptr)
        {
            delete[] this->payload;
        }

        this->byte = source.byte;
        this->payload = source.payload;

        source.payload = nullptr;
        source.byte = 0;
        return *this;
    }

    ~NetworkPacket()
    {
        std::cout << "Desctructor:: called" << "\n";
        if (payload != nullptr)
        {
            delete[] payload;
        }
    }
};

int main()
{
    std::cout << "=== Network Packet Main Program === \n\n";
    NetworkPacket packet1(100);
    NetworkPacket packet2 = packet1; // copy constructor
    NetworkPacket packet3(50);
    packet3 = packet1;                          // copy assignment operator
    NetworkPacket packet4 = std::move(packet1); // move constructor
    NetworkPacket packet5(200);
    packet5 = std::move(packet2); // move assignment operator
    return 0;
}