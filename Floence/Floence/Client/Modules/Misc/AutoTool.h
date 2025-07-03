#pragma once

class AutoTool : public Module
{
public:
    AutoTool(int keybind = 7, bool enabled = true) :
        Module("AutoTool", "Misc", "Select best tool automatically for you", keybind, enabled)
    {
        addBool("Durability Exploit", "it gives you infinite durability", &InfDurability);
        //
        addBool("Spoof", "lol", &test);
    }

    bool test = false;
    int previousSlot = 0;
    bool isMining = false;
    bool InfDurability = false;
    int currentServerSlot = 0;
    int ToolSlot = 0;

    int getInvalidSlot() {
        auto player = Address::getLocalPlayer();
        PlayerInventory* supplies = player->getSupplies();
        for (int i = 0; i < 8; i++) {
            int i2 = 8 - i;
            if (i2 != supplies->hotbarSlot && i2 != currentServerSlot && i2 != ToolSlot && i2 != previousSlot) return i2;
        }
        return 0;
    }

    int getBestBreakingTool(Block* block) {
        auto player = Address::getLocalPlayer();
        if (!player) return 0;

        GameMode* gamemode = player->getGamemode();
        if (!gamemode) return 0;

        PlayerInventory* supplies = player->getSupplies();
        int previousSlot = supplies->hotbarSlot;

        int slot = 0;
        float fastestSpeed = 0;
        if (InfDurability) supplies->hotbarSlot = getInvalidSlot();
        for (int i = 0; i < 36; i++)
        {
            auto itemStack = player->getSupplies()->inventory->getItem(i);
            if (!itemStack || !itemStack->item) continue;
            supplies->hotbarSlot = i;
            float speed = gamemode->getDestroyRate(*block);

            if (fastestSpeed < speed)
            {
                fastestSpeed = speed;
                slot = i;
            }
        }
        
        supplies->hotbarSlot = previousSlot;
        Misc::SpoofSwitch(slot);
        return slot;
    }

    void onEnabled() override {
        auto player = Address::getLocalPlayer();
        if (player == nullptr) return;
        PlayerInventory* supplies = player->getSupplies();
        isMining = false;
        previousSlot = supplies->hotbarSlot;
    }

    void onEvent(ActorBaseTickEvent* event) override {
        auto player = Address::getLocalPlayer();
        if (player == nullptr) return;

        GameMode* gamemode = player->getGamemode();
        if (!gamemode) return;

        BlockSource* source = Address::getBlockSource();
        if (!source) return;

        PlayerInventory* supplies = player->getSupplies();

        // Breaking block
        if (0 < gamemode->getBreakProgress()) {
            if (!isMining) {
                HitResult* hitResult = player->getLevel()->getHitResult();
                Vector3<int> blockPos = hitResult->IBlockPos;
                Block* block = source->getBlock(blockPos);
                if (!block) return;
                if (test) {
                    previousSlot = supplies->hotbarSlot;
                    Misc::SpoofSwitch(getBestBreakingTool(block));
                    isMining = true;
                    if (gamemode->getBreakProgress() == 0.96f) {
                        gamemode->destroyBlock(blockPos, hitResult->BlockFace);
                    }
                }
                else {
                    previousSlot = supplies->hotbarSlot;
                    supplies->hotbarSlot = getBestBreakingTool(block);
                    isMining = true;
                }
            }
        }
        else if (isMining) {
            supplies->hotbarSlot = previousSlot;
            isMining = false;
        }

    }
};