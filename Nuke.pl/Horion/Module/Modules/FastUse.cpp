#include "FastUse.h"

using namespace std;
FastUse::FastUse() : IModule(0, Category::MISC, "Description") {
}

const char* FastUse::getModuleName() {
    return "FastUse";
}

vector<string> allowedItems = {
    "snowball",
    "bottle",
    "splash",
    "rod",
    "bow"
};

void FastUse::onTick(GameMode* gm) {
    auto player = Game.getLocalPlayer();
    if (player == nullptr || gm == nullptr) return;

    if (Game.canUseMoveKeys() && Game.isRightClickDown()) {
        ItemStack* stack = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
        if (stack->item != nullptr && (* stack->item)->itemId != 0) {

            string itemName = stack->getItem()->name.getText();
            for (auto item : allowedItems) {
                if (itemName.find(item) != string::npos) {
                    gm->useItem(*stack);
                }
            }
        }
    }
}