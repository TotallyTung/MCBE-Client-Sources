//
// Created by vastrakai on 10/7/2023.
//

#include <Prax/Utils/String.h>
#include <Prax/UI/Notifications.h>
#include "AutoSpellbook.h"

uint64_t AutoSpellbook::LastItemUsed = 0;

void AutoSpellbook::OnTick() {
    if (LastItemUsed + (uint64_t)Delay.Value > Misc::GetCurrentMs()) {
        return;
    }

    auto player = Minecraft::ClientInstance->ThePlayer();
    // Check the absorption

    if (player->getHealth() > 12) {
        return; // min health for spellbook is 16
    }
    ItemStack* itemStack = nullptr;
    int itemSlot = -1;

    for (int i = 0; i < 36; i++) {
        auto item = player->supplies->GetItemAtSlot(i);

        if (item && item->item) {
            if (item->GetCustomName() == "§r§dSpell of Life§7 [Use]") {
                itemStack = item;
                itemSlot = i;
                break;
            }
        }
    }

    if (itemStack == nullptr) {
        return;
    }

    std::string a = itemStack->GetCustomName();
    if (String::ContainsIgnoreCase(a, "§r§dSpell of Life§7 [Use]")) {
        Misc::SwitchAndEquip(itemSlot);
        LastItemUsed = Misc::GetCurrentMs();
        if (ShowNotification.Enabled) Notifications::Notify("AutoSpellbook", "Used spellbook!", NotificationType::Info, 3.f);
    }
}
