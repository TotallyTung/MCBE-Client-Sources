#pragma once

class Manager : public Module {
private:
    bool openInv = false;

    bool check = false;

public:
    Manager(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Manager", "Player", "Display the client watermark", keybind, enabled)
    {
        registerBoolSetting("OpenInv", "need inventory menu", &openInv);
    }

    void onEnabled()
    {
        check = false;
    }



    void onEvent(LayerUpdateEvent* event) override
    {
        if (UILayer::Is(event->GuiLayer, { UILayer::Ingame_InventoryScreen }))
        {
            check = true;
        }
        else {
            check = false;
        }
    }


    bool dropSlot() {
        auto supplies = Game::GetLocalPlayer()->getSupplies();
        auto inv = supplies->inventory;
        auto prevSlot = supplies->hotbarSlot;

        for (int i = 0; i < 8; i++) {
            ItemStack* stack = inv->getItem(i);
            if (stack->item) {
                inv->dropSlot(i);
                return true;
            };
        }
    

        return false;
    }

    void onEvent(UpdateEvent* event) {
        if (openInv && check) {
            dropSlot();
        }
        else
        {
            dropSlot();
        }
    }
};