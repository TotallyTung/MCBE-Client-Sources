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
        char errorMsg[] = "[%snhack%s] %sInvalid command! Usage: .(item) <hold/h>";
        mc.DisplayClientMessage(errorMsg, BLUE, WHITE, RED);
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
                sprintf(msg, "[%snhack%s] %s Item ID is -> : %d", BLUE, WHITE, GREEN, itemId);
                mc.DisplayClientMessage(msg);
            }
            else {
                mc.DisplayClientMessage("[%snhack%s] %sYou are not holding a valid item!", BLUE, WHITE, RED);
            }
        }
        else {
            mc.DisplayClientMessage("[%snhack%s] %sInvalid command! Usage: .(item) hold", BLUE, WHITE, RED);
        }
        break; 
    default:
        mc.DisplayClientMessage("[%snhack%s] %sInvalid command!", BLUE, WHITE, RED);
        break;
    }
}
