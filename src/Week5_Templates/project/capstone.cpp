#include <iostream>
#include <utility>
#include <cstring>

class MyString
{
public:
    char *data;
    int length;

public:
    MyString(const char *str = "") // default constructor with default parameter
    {
        length = std::strlen(str);
        data = new char[length + 1]; // +1 for null terminator
        std::strcpy(data, str);
        std::cout << "[+] Created: " << data << "\n";
    }

    ~MyString()
    {
        std::cout << "[-] Deleted: " << data << "\n";
        delete[] data;
    }

    MyString(const MyString &other)
    {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data); // copy the string data from the other object
        std::cout << "[COPY CONSTRUCTOR] Created: " << data << "\n";
    }

    MyString &operator=(const MyString &other)
    {
        std::cout << "[COPY ASSIGNMENT] Assigned: " << other.data << "\n";
        if (this == &other)
        {
            return *this;
        }
        if (data != nullptr)
        {
            delete[] data;
        }

        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);

        return *this;
    }

    bool operator==(const MyString &other) const
    {
        return std::strcmp(this->data, other.data) == 0;
    }

    char &operator[](int index)
    {
        if (index < 0 || index >= length)
        {
            std::cout << "[Error]: Out of bound \n";
            return data[0];
        }
        return data[index];
    }

    MyString operator+(const MyString &other) const
    {
        int newLength = this->length + other.length; // calculate the length of the concatenated string
        char *temp = new char[newLength + 1];
        std::strcpy(temp, this->data); // copy the first string data to temp
        std::strcat(temp, other.data); // concatenate the second string data to temp
        MyString result(temp);         // create a new MyString object with the concatenated string

        delete[] temp;
        return result;
    }

    friend std::ostream &operator<<(std::ostream &os, const MyString &str)
    {
        os << str.data;
        return os;
    }
};

template <typename T>
class MyVector
{
private:
    int capacity;
    int currentSize;
    T *data;

public:
    MyVector(int cap = 5)
    {
        capacity = cap;
        currentSize = 0;
        data = new T[capacity];
    }

    ~MyVector()
    {
        delete[] data;
    }

    MyVector(const MyVector &other)
    {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];

        for (int i = 0; i < currentSize; i++)
        {
            data[i] = other.data[i];
        }
    }

    MyVector(MyVector &&other) noexcept
    {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = std::move(other.data);

        other.data = nullptr;
        other.capacity = 5;
        other.currentSize = 0;
    }

    MyVector &operator=(const MyVector &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (data != nullptr)
        {
            delete[] data;
        }

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    MyVector &operator=(MyVector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (data != nullptr)
        {
            delete[] data;
        }

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = std::move(other.data);

        other.data = nullptr;
        other.currentSize = 0;
        other.capacity = 5;

        return *this;
    }

    void push(const T &element)
    {
        if (currentSize < capacity)
        {
            data[currentSize] = element;
            currentSize++;
        }
        else
        {
            std::cout << "[!] Vector is full! (Resizing logic goes here)\n";
        }
    }

    void print()
    {
        std::cout << "[ ";
        for (int i = 0; i < currentSize; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << "]\n";
    }
};

int main()
{
    MyVector<MyString> stringVector(3);
    MyString greeting("Hello");
    MyString world("World");
    MyString cpp("C++");
    stringVector.push(greeting);
    stringVector.push(world);
    stringVector.push(cpp);
    stringVector.print();
    MyVector<MyString> backupDialog = stringVector;
    return 0;
}