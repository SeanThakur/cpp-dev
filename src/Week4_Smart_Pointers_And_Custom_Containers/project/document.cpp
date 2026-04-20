#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <vector>

class Document
{
public:
    std::string name;
    Document(std::string n) : name(n)
    {
        std::cout << "[+] Document created \n";
    }
    ~Document()
    {
        std::cout << "[-] Document destroyed \n";
    }
};

void printDocumet(const Document *doc)
{
    if (doc != nullptr)
    {
        std::cout << "--> Printing thid doc " << doc->name << "\n";
    }
}

void shredDocument(std::unique_ptr<Document> doc)
{
    std::cout << "--> Shredding the document... " << doc->name << "\n";
}

class Vault
{
private:
    std::vector<std::unique_ptr<Document>> archives;

public:
    void storedDocumetn(std::unique_ptr<Document> doc)
    {
        std::cout << "[+] Document stored in Vault " << doc->name << "\n";
        archives.push_back(std::move(doc));
    }

    void emptyVault()
    {
        archives.clear();
    }

    void inspectAll()
    {
        for (size_t i = 0; i < archives.size(); i++)
        {
            printDocumet(archives[i].get());
        }
    }

    std::unique_ptr<Document> extractLastDocument()
    {
        if (archives.empty())
        {
            return nullptr;
        }
        std::unique_ptr<Document> stolenDoc = std::move(archives.back());
        archives.pop_back();
        return stolenDoc;
    }
};

int main()
{
    std::cout << "---Main Program--- \n";
    std::unique_ptr<Document> doc1 = std::make_unique<Document>("TopSecret.pdf");
    std::unique_ptr<Document> doc2 = std::make_unique<Document>("Financials.xlsx");
    std::unique_ptr<Document> doc3 = std::make_unique<Document>("Passwords.txt");

    Vault v1;
    v1.storedDocumetn(std::move(doc1));
    v1.storedDocumetn(std::move(doc2));
    v1.storedDocumetn(std::move(doc3));

    std::cout << "--- Executing Heist --- \n";
    std::unique_ptr<Document> stolenDoc = v1.extractLastDocument();

    std::cout << "--- Inspecting all documents --- \n";
    v1.inspectAll();

    std::cout << "--- Check Value after Heist --- \n";
    shredDocument(std::move(stolenDoc));

    std::cout << "--- Calling emptyVault() --- \n";
    v1.emptyVault();
    std::cout << "----END Main Programm---- \n";
    return 0;
}