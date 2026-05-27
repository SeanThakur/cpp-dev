#include <iostream>
#include <functional>
#include <string>

class UIButton
{
private:
    std::string label;
    std::function<void()> onClickAction;

public:
    UIButton(const std::string &name)
    {
        label = name;
        onClickAction = nullptr;
    }

    void bindAction(std::function<void()> task)
    {
        onClickAction = task;
        std::cout << "[UI] Action bound to button: [" << label << "]\n";
    }

    void executeClickableAction()
    {
        if (onClickAction)
        {
            onClickAction();
        }
        else
        {
            std::cout << "Button on click action for " << label << " does nothing \n\n";
        }
    }
};

int main()
{
    std::cout << "--- THE COMMAND PATTERN ---\n\n";
    UIButton saveButton("Save Button");
    UIButton exitButton("Exit Button");
    UIButton redButton("RedButton");

    saveButton.bindAction([]()
                          {
        std::cout << "    -> [Engine] Loading Level 1...\n";
        std::cout << "    -> [Engine] Spawning Player...\n"; });

    exitButton.bindAction([]()
                          {
        std::cout << "    -> [Engine] Saving game state...\n";
        std::cout << "    -> [Engine] Player Exited...\n"; });

    saveButton.executeClickableAction();
    exitButton.executeClickableAction();
    redButton.executeClickableAction();
    return 0;
}