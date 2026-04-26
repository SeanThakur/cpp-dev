#include <iostream>
#include <memory>
#include <vector>
#include <string>

class IStorageNode
{
public:
    virtual ~IStorageNode()
    {
        std::cout << "[-] Storage Node is destroyed \n";
    }

    virtual void save(const std::string &data) = 0;
    virtual std::string getNodeName() = 0;
};

class SecureSQLNode : public IStorageNode
{
public:
    ~SecureSQLNode() override
    {
        std::cout << "[-] Secure Sql Node is destroyed \n";
    }

    void save(const std::string &data) override
    {
        std::cout << "   -> [SQL] Encrypting and writing to disk: '" << data << "'\n";
    }

    std::string getNodeName() override
    {
        return "Secure SQL Node";
    }
};

class FastCacheNode : public IStorageNode
{
public:
    ~FastCacheNode() override
    {
        std::cout << "[-] Fast Cache Node is destroyed \n";
    }

    void save(const std::string &data) override
    {
        std::cout << "   -> [CACHE] FastCacheNode flushing RAM and closing... '" << data << "'\n";
    }

    std::string getNodeName() override
    {
        return "FastCache";
    }
};

class DatabaseCluster
{
private:
    std::vector<std::unique_ptr<IStorageNode>> nodes;

public:
    void attachNode(std::unique_ptr<IStorageNode> node)
    {
        nodes.push_back(std::move(node));
    }

    void broadcastData(const std::string &data)
    {
        for (auto &node : nodes)
        {
            node->save(data);
        }
    }
};

int main()
{
    std::cout << "\n *** MAIN PROGRAMM ***\n";
    DatabaseCluster cluster;
    cluster.attachNode(std::make_unique<SecureSQLNode>());
    cluster.attachNode(std::make_unique<FastCacheNode>());
    cluster.broadcastData("Sample Data");
    std::cout << "\n *** END PROGRAMM ***\n";
    return 0;
}