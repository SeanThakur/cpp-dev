#include <iostream>
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
};

int main()
{
    MyString str1("Hello");
    MyString str2(" World");
    MyString str5("Hell");
    MyString word("Cello");
    MyString str3 = str1 + str2; // operator+ will create a new MyString object with the concatenated string
    std::cout << "Concatenated String: " << str3.data << "\n";
    MyString str4 = str3; // copy constructor will be called
    std::cout << "Copied String: " << str4.data << "\n";
    std::cout << "str1 == str5: " << (str1 == str5) << "\n";
    word[0] = 'B'; // operator[] will allow us to modify the string data
    std::cout << "Modified word: " << word.data << "\n";
    return 0;
}