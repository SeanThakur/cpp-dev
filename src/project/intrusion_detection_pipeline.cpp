// The Zero-Copy Intrusion Detection Pipeline

#include <iostream>
#include <memory>
#include <utility>
#include <cstring>
#include <string_view>
#include <vector>

namespace CyberOps
{
    class NetworkPlayer
    {
    private:
        int length;
        char *data;

    public:
        NetworkPlayer(const char *rawTraffic)
        {
            length = std::strlen(rawTraffic);
            data = new char[length + 1];
            std::strcpy(data, rawTraffic);
            std::cout << "[+] Playload Allocated \n";
        }

        ~NetworkPlayer()
        {
            if (data != nullptr)
            {
                delete[] data;
                std::cout << "[-] Plaload Destroyed \n";
            }
        }

        NetworkPlayer(const NetworkPlayer &other) = delete;
        NetworkPlayer &operator=(const NetworkPlayer &other) = delete;

        NetworkPlayer(NetworkPlayer &&other) noexcept
        {
            std::cout << "[+] Move constructor \n";
            length = other.length;
            data = other.data;

            other.length = 0;
            other.data = nullptr;
        }

        NetworkPlayer &operator=(NetworkPlayer &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            if (data != nullptr)
            {
                delete[] data;
            }

            length = other.length;
            data = other.data;
            other.length = 0;
            other.data = nullptr;
            return *this;
        }

        const char *getTraffic() const
        {
            return (data != nullptr) ? data : "EMPTY";
        }
    };

    class IPacketFilter
    {
    public:
        virtual ~IPacketFilter() = default;

        virtual bool inspect(const NetworkPlayer &packet) = 0;
    };

    class SqlInjectionFilter : public IPacketFilter
    {
    public:
        bool inspect(const NetworkPlayer &packet) override
        {
            std::cout << "   [FILTER] SQL Injection Check... \n";

            std::string_view traffic = packet.getTraffic();
            // Check if the malicious string exists in the payload
            if (traffic.find("DROP TABLE") != std::string_view::npos)
            {
                std::cout << "Straight to JAIL. \n";
                return false;
            }
            std::cout << "Passed. \n";
            return true;
        }
    };

    class AuthFilter : public IPacketFilter
    {
    public:
        bool inspect(const NetworkPlayer &packet) override
        {
            std::cout << "   [FILTER] Authorization Check... \n";

            std::string_view traffic = packet.getTraffic();
            if (traffic.find("Authorization") == std::string_view::npos)
            {
                std::cout << "No AUTH Found \n";
                return false;
            }

            std::cout << "Passed. \n";
            return true;
        }
    };

    template <typename T>
    class SecurityPipeline
    {
    private:
        std::vector<std::unique_ptr<IPacketFilter>> filters;

    public:
        SecurityPipeline &operator+(std::unique_ptr<IPacketFilter> newFilter)
        {
            filters.push_back(std::move(newFilter));
            return *this;
        }
        bool process(std::unique_ptr<T> payload)
        {
            std::cout << "\n[PIPELINE] Incoming Traffic Detected...\n";
            for (const auto &filter : filters)
            {
                if (filter->inspect(*payload) == false)
                {
                    std::cout << "[PIPELINE] Threat neutralized. Dropping packet.\n";
                    return false;
                }
            }

            return true;
        }
    };
}

int main()
{
    std::cout << "--- ZERO-COPY SECURITY PIPELINE BOOTING ---\n";

    // 1. Instantiate our Template Pipeline
    // We tell it to expect our 'NetworkPlayer' class
    CyberOps::SecurityPipeline<CyberOps::NetworkPlayer> firewall;

    // 2. Build the Pipeline using our Overloaded Operator (+)
    // We chain them together elegantly!
    firewall + std::make_unique<CyberOps::SqlInjectionFilter>() + std::make_unique<CyberOps::AuthFilter>();

    std::cout << "\n--- TEST 1: LEGITIMATE TRAFFIC ---\n";
    // We dynamically allocate the payload and hand exclusive ownership to the firewall
    auto safePacket = std::make_unique<CyberOps::NetworkPlayer>("POST /data HTTP/1.1\r\nAuthorization: Bearer 12345");
    firewall.process(std::move(safePacket));

    std::cout << "\n--- TEST 2: MALICIOUS TRAFFIC ---\n";
    auto hackPacket = std::make_unique<CyberOps::NetworkPlayer>("GET /admin?query=DROP TABLE users; HTTP/1.1");
    firewall.process(std::move(hackPacket));

    std::cout << "\n--- TEST 3: MISSING AUTHENTICATION ---\n";
    auto noAuthPacket = std::make_unique<CyberOps::NetworkPlayer>("POST /data HTTP/1.1\r\nUser: Anonymous");
    firewall.process(std::move(noAuthPacket));

    std::cout << "\n--- SERVER SHUTDOWN ---\n";
    return 0;
}