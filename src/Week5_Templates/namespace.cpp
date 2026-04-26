#include <iostream>

namespace CyberLog
{
    class Logger
    {
    public:
        void log()
        {
            std::cout << "CyberLog logger! \n";
        }
    };
}

namespace AuditLog
{
    class Logger
    {
    public:
        void log()
        {
            std::cout << "AuditLog logger! \n";
        }
    };
}

int main()
{
    CyberLog::Logger MyLogger;
    MyLogger.log();

    AuditLog::Logger netLogger;
    netLogger.log();
    return 0;
}