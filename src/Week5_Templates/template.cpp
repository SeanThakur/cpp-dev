#include <iostream>

template <typename T>
T getMAx(T a, T b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

template <typename T>
class Node
{
public:
    T data;

    Node(T d) : data(d)
    {
        std::cout << "[+] Constructor called with data " << data << "\n";
    }

    T getNode()
    {
        return data;
    }
};

int main()
{
    int data = getMAx<int>(5, 3);
    std::cout << "Max value: " << data << std::endl;

    Node<std::string> myNode("SEAN");
    std::cout << "My Node class " << myNode.getNode() << std::endl;
    return 0;
}