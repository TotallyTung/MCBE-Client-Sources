#pragma once

class Stealer : public Module
{
public:
    Stealer(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Stealer", "Player", "Steal items out of chests", keybind, enabled)
    {
        registerFloatSetting("Min", "Min Speed", &speedMin, 0, 100);
        registerFloatSetting("Max", "Max Speed", &speedMax, 0, 100);
    }

    float speedMin = 24;
    float speedMax = 48;

    int item = 0;
    int maxItem = 56;

    bool inChest = false;

    void onEvent(LayerUpdateEvent* event) override
    {
        if (UILayer::Is(event->GuiLayer, {
                UILayer::Ingame_LargeChestScreen,
                UILayer::Ingame_SmallChestScreen,
                UILayer::Ingame_ShulkerBoxScreen,
                UILayer::Ingame_BarrelScreen,
                UILayer::Ingame_EnderChestScreen, // Added
            }))
        {
            inChest = true;

            if (UILayer::Is(event->GuiLayer, UILayer::Ingame_LargeChestScreen))
            {
                maxItem = 54;
            }

            if (UILayer::Is(event->GuiLayer, {
                    UILayer::Ingame_SmallChestScreen,
                    UILayer::Ingame_ShulkerBoxScreen,
                    UILayer::Ingame_BarrelScreen,
                    UILayer::Ingame_EnderChestScreen// Added
                }))
            {
                maxItem = 27;
            }
        }

        if (UILayer::Is(event->GuiLayer, {
                UILayer::Ingame_InventoryScreen,
                UILayer::Ingame_CanMoveScreen
            }))
        {
            inChest = false;
        }
    }

    inline float RandomFloat(float a, float b) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }

    void onEvent(ContainerTickEvent* event) override
    {
        if (!inChest)
            return;

        if (TimeUtil::hasTimeElapsed("Cs", 500, false))
            item = 0;

        ContainerScreenController* controller = event->Controller;

        if (!controller)
            return;

        float speed = RandomFloat(speedMin * 2, speedMax * 2);

        if (TimeUtil::hasTimeElapsed("Cs", 1000 / speed, true))
        {
            controller->shiftClickItems(Containers::Container, item);

            if (item == maxItem)
            {
                item = -1;

                controller->closeContainer();
            }

            item++;
        }
    }

    std::string getModeName() override
    {
        std::ostringstream oss;
        oss.precision(0);
        oss << " " << std::fixed << speedMin << " " << std::fixed << speedMax;
        return oss.str();
    }
};