#include "Switcher.h"

Switcher::Switcher() : Module("Switcher", "Switch to Swords when tryna attack.", Category::COMBAT) {
}

void Switcher::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    Container* inventory = localPlayer->playerInventory->container;
    if (!inventory) return;

    static int lastSlot = localPlayer->playerInventory->selectedSlot;
    std::vector<int> swordSlots; 

    for (int i = 0; i < 9; i++) {
        auto itemStack = inventory->getItem(i);
        if (itemStack && itemStack->item) {
            std::string itemName = itemStack->item->texture_name;

            if (itemName.find("sword") != std::string::npos) {
                swordSlots.push_back(i);
            }
        }
    }

    static int currentIndex = 0;
    if (!swordSlots.empty()) {
        for (int j = 0; j < 5; ++j) {
            localPlayer->playerInventory->selectedSlot = swordSlots[currentIndex];
            currentIndex = (currentIndex + 1) % swordSlots.size(); 
        }
    }
}