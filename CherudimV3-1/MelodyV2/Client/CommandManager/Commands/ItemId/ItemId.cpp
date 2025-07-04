#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "ItemId.h"
#include "../../CommandManager.h"
#include "../../../Client.h"

ItemId::ItemId() : Command("ItemId", "itemid settings", { "item" }, "<u got some stupititys>") {}

void ItemId::execute(const std::vector<std::string>& args) {
    static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
    configMgr = new ConfigManager();
    if (args.size() < 2) {
        char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .(item) <hold/h>";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }

    std::string subCommand = args[1];
    switch (subCommand[0]) {
    case 'h':
        if (subCommand == "hold") {
            auto* plrInv = mc.getLocalPlayer()->getPlayerInventory();
            auto* invnentory = plrInv->inventory;
            auto selectedsot = plrInv->selectedSlot;
            auto* itemStack = invnentory->getItemStack(selectedsot);

            if (itemStack->isValid()) {
                const int itemId = itemStack->getItemPtr()->itemId;
                char msg[256];
                sprintf(msg, "[%sMelody%s] %s Item ID is -> : %d", DARK_PURPLE, WHITE, GREEN, itemId);
                mc.DisplayClientMessage(msg);
            }
            else {
                mc.DisplayClientMessage("[%sMelody%s] %sYou are not holding a valid item!", DARK_PURPLE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(item) hold", DARK_PURPLE, WHITE, RED);
        }
        break; 
    default:
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command!", DARK_PURPLE, WHITE, RED);
        break;
    }
}
