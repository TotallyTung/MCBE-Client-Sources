#pragma once

class ChestStealer : public Module
{
public:
    ChestStealer(int keybind = 7, bool enabled = true) :
        Module("ChestStealer", "Player", "Steal items out of chests", keybind, enabled)
    {
        addSlider("SPS", "How many items are moved a second", &SPS, 1, 100);
    }

    float SPS = 100;

    int item = 0;
    int maxItem = 56;

    void onEvent(ContainerTickEvent* event) override
    {
        if (TimeUtils::hasTimeElapsed("Cs", 500, false))
            item = 0;

        ContainerScreenController* controller = event->Controller;

        if (!controller)
            return;

        Player* player = Address::getClientInstance()->getLocalPlayer();
        PlayerInventory* playerinventory = player->getSupplies();
        Inventory* inventory = playerinventory->inventory;


        if (Global::LayerName != "chest.small_chest_screen" && Global::LayerName != "chest.large_chest_screen" && Global::LayerName != "chest.ender_chest_screen")
            return;

        if (TimeUtils::hasTimeElapsed("Cs", 1000 / SPS, true)) {
            for (int i = 0; i < 56; ++i)
            {
                if (inventory->getItem(i)->item != nullptr) {
                    controller->handleTakeAll(Containers::Container, i);
                    return;
                }
                else continue;
            }
            controller->closeContainer();
        }
    }

    std::string getModeName() override {
        return " Delayed";
    }
};
