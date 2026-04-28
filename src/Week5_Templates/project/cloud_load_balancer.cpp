#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace CloudeOPS
{
    class Server
    {
    private:
        std::string server_name;
        int active_connection;

    public:
        Server(std::string name, int conn) : server_name(name), active_connection(conn)
        {
            std::cout << "[+] Server node is created " << server_name << " \n";
        }

        std::string getServerName()
        {
            return server_name;
        }

        int getActiveConnections()
        {
            return active_connection;
        }

        bool operator>(const Server &other) const
        {
            return this->active_connection > other.active_connection;
        }
    };

    template <typename T>
    class ClusterNode
    {
    private:
        std::vector<std::shared_ptr<T>> items;

    public:
        void attachItem(std::shared_ptr<T> data)
        {
            items.push_back(data);
        }

        std::shared_ptr<T> getBusiestConnection()
        {
            if (items.empty())
            {
                return nullptr;
            }

            auto tempItem = items[0];

            for (const auto &item : items)
            {
                if (*item > *tempItem)
                {
                    tempItem = item;
                }
            }

            return tempItem;
        }
    };
}

int main()
{
    CloudeOPS::ClusterNode<CloudeOPS::Server> cluster;

    std::shared_ptr<CloudeOPS::Server> alpha = std::make_shared<CloudeOPS::Server>("Alpha", 500);
    std::shared_ptr<CloudeOPS::Server> beta = std::make_shared<CloudeOPS::Server>("Beta", 8500);
    std::shared_ptr<CloudeOPS::Server> gamma = std::make_shared<CloudeOPS::Server>("Gamma", 1200);

    cluster.attachItem(alpha);
    cluster.attachItem(beta);
    cluster.attachItem(gamma);

    std::shared_ptr<CloudeOPS::Server> busiestNode = cluster.getBusiestConnection();

    if (busiestNode)
    {
        std::cout << "[SYSTEM] Routing traffic to heaviest node:\n";
        std::cout << "         -> Node: " << busiestNode->getServerName() << "\n";
        std::cout << "         -> Load: " << busiestNode->getActiveConnections() << " connections\n";
    }
    else
    {
        std::cout << "[SYSTEM] Cluster is empty. No routing available.\n";
    }

    std::cout << "--- SHUTTING DOWN ---\n";
    return 0;
}