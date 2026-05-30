#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>

enum class EventType
{
    PlayerTookDamage,
    PlayerDied,
    BossDefeated
};

struct Event
{
    EventType type;
    int entityID;
    int dataValue;
};

class EventBus
{
private:
    std::map<EventType, std::vector<std::function<void(const Event &)>>> listeners;
    std::vector<Event> eventQueue;

public:
    void subscribe(EventType event, std::function<void(const Event &)> callback)
    {
        listeners[event].push_back(callback);
    }

    void postEvent(const Event &e)
    {
        eventQueue.push_back(e);
        std::cout << "[EventBus] Event queued for later dispatch.\n";
    }

    void dispatchAll()
    {
        if (eventQueue.empty())
        {
            return;
        }
        for (const auto &e : eventQueue)
        {
            if (listeners.count(e.type))
            {
                for (const auto &action : listeners[e.type])
                {
                    action(e);
                }
            }
        }
        eventQueue.clear();
    }
};

class AudioSystem
{
public:
    void init(EventBus &bus)
    {
        bus.subscribe(EventType::PlayerDied, [this](const Event &e)
                      { std::cout << "    [Audio] Playing 'sad_violins.wav'\n"; });

        bus.subscribe(EventType::PlayerTookDamage, [this](const Event &e)
                      { std::cout << "    [Audio] Playing 'grunt.wav' (Damage Taken: " << e.dataValue << ")\n"; });
    }
};

class QuestSystem
{
private:
    int boss = 2;

public:
    void init(EventBus &bus)
    {
        bus.subscribe(EventType::BossDefeated, [this](const Event &e)
                      {
            boss--;
            std::cout << "    [Quest] Boss ID " << e.entityID << " defeated. Bosses left: " << boss << "\n";
            if(boss == 0) {
                std::cout << "    [Quest] -> QUEST COMPLETE! Granting 10,000 XP.\n";
            } });
    }
};

int main()
{
    std::cout << "--- PUB-SUB EVENT BUS ---\n\n";
    // 1. Boot up the Engine
    EventBus masterBus;
    AudioSystem audio;
    QuestSystem quests;

    // 2. Subsystems bind their Lambdas to the Bus
    audio.init(masterBus);
    quests.init(masterBus);

    std::cout << ">> SIMULATING GAMEPLAY FRAME 1...\n";
    // The player steps on a trap. They don't talk to Audio, they just post an Event.
    masterBus.postEvent({EventType::PlayerTookDamage, 777, 45});

    // The player kills the boss.
    masterBus.postEvent({EventType::BossDefeated, 99, 0});

    // End of Frame 1
    masterBus.dispatchAll();

    std::cout << "\n>> SIMULATING GAMEPLAY FRAME 2...\n";
    // The player takes massive damage and dies
    masterBus.postEvent({EventType::PlayerTookDamage, 777, 999});
    masterBus.postEvent({EventType::PlayerDied, 777, 0});

    // End of Frame 2
    masterBus.dispatchAll();
    return 0;
}