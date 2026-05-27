#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <map>

class EventDispatcher
{
private:
    std::map<std::string, std::vector<std::function<void()>>> listeners;

public:
    void subscribe(const std::string &eventName, std::function<void()> action)
    {
        listeners[eventName].push_back(action);
    }

    void trigger(const std::string &eventName)
    {
        std::cout << "\n>> Event Triggered: [" << eventName << "]\n";
        for (auto &callback : listeners[eventName])
        {
            callback();
        }
    }
};

int main()
{
    EventDispatcher disapatcher;
    disapatcher.subscribe("PlayerDeath", []()
                          { std::cout << " [Audio] Playing sad trombone sound.\n"; });
    disapatcher.subscribe("PlayerDeath", []()
                          { std::cout << " [UI] Fading screen to black.\n"; });
    disapatcher.subscribe("NewPlayer", []()
                          { std::cout << " [UI] Welcoming screen for new player.\n"; });
    disapatcher.subscribe("NewPlayer", []()
                          { std::cout << " [Audio] Playing welcom song.\n"; });
    disapatcher.trigger("PlayerDeath");
    disapatcher.trigger("NewPlayer");
    return 0;
}