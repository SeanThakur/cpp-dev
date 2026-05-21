#include <iostream>
#include <vector>
#include <string>
#include <memory>

class DatabaseConnection
{
private:
    std::string dbName;

public:
    DatabaseConnection(const std::string &name) : dbName(name)
    {
        std::cout << "DatabaseConnection to " << dbName << " established." << std::endl;
    }
    ~DatabaseConnection()
    {
        std::cout << "DatabaseConnection to " << dbName << " closed." << std::endl;
    }
    void executeQuery(const std::string &query)
    {
        std::cout << "Executing query on " << dbName << ": " << query << std::endl;
    }
};

template <typename T>
class ResourceManager
{
private:
    std::vector<std::unique_ptr<T>> pool;

public:
    ResourceManager(const ResourceManager &other) = delete;            // Disable copy constructor
    ResourceManager &operator=(const ResourceManager &other) = delete; // Disable copy assignment operator
    ResourceManager() = default;

    void addResource(std::unique_ptr<T> resource)
    {
        pool.push_back(std::move(resource));
        std::cout << "Resource added to the pool." << std::endl;
    }

    T *getResouce(size_t index)
    {
        if (index < pool.size())
        {
            return pool[index].get();
        }
        std::cerr << "Index out of bounds." << std::endl;
        return nullptr;
    }
};

int main()
{
    {
        std::cout << ">> INITIALIZING ENGINE SUBSYSTEMS...\n";
        ResourceManager<DatabaseConnection> dbManager;
        auto db1 = std::make_unique<DatabaseConnection>("MainDB");
        auto db2 = std::make_unique<DatabaseConnection>("AnalyticsDB");
        std::cout << "\n>> ABSORBING RESOURCES INTO MANAGER...\n";
        dbManager.addResource(std::move(db1));
        dbManager.addResource(std::move(db2));
        std::cout << "\n>> BORROWING RESOURCE FOR GAMEPLAY...\n";

        DatabaseConnection *db = dbManager.getResouce(0);
        if (db)
        {
            db->executeQuery("SELECT * FROM players");
        }
        std::cout << "\n>> CRITICAL ENGINE FAILURE! SHUTTING DOWN SCOPE...\n";
    }
    return 0;
}