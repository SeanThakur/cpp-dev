#include <iostream>
#include <memory>

struct DatabaseConnection
{
    void open()
    {
        std::cout << "[+] DB Connected \n";
    }

    void closeConnection()
    {
        std::cout << "[-] DB Disconnected \n";
    }
};

int main()
{
    {
        DatabaseConnection *rawDb = new DatabaseConnection();
        rawDb->open();

        auto dbDeleter = [](DatabaseConnection *db)
        {
            db->closeConnection();
            delete db;
        };
        std::unique_ptr<DatabaseConnection, decltype(dbDeleter)> safeDb(rawDb, dbDeleter);
    }
    return 0;
}