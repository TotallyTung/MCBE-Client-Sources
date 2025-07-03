#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <Utils/MemUtils.hpp>
#include <SDK/SigManager.hpp>


class GameMode {
public:
    CLASS_FIELD(class Actor*, mPlayer, 0x8);
    CLASS_FIELD(float, mBreakProgress, 0x24);

    // for lua bindings (only for funcs that require a bool reference)
    bool startDestroyBlockByLua(glm::ivec3& position, int blockSide) {
        bool isDestroyed = false;
        startDestroyBlock(position, blockSide, isDestroyed);
        return isDestroyed;
    }

    bool continueDestroyBlockByLua(glm::ivec3 const& position, unsigned int blockSide, glm::vec3 const& playerPosition) {
        bool isDestroyed = false;
        continueDestroyBlock(position, blockSide, playerPosition, isDestroyed);
        return isDestroyed;
    }

    // realistically, i don't think this vTable will ever change so we can leave it here lol
    virtual ~GameMode() = 0;
    virtual void startDestroyBlock(glm::ivec3& position, int blockSide, bool& isDestroyedOut) = 0;
    virtual void destroyBlock(glm::ivec3& position, int blockSide) = 0;
    virtual void continueDestroyBlock(glm::ivec3 const& position, unsigned int blockSide, glm::vec3 const& playerPosition, bool&) = 0;
    virtual void stopDestroyBlock(glm::ivec3 const& position) = 0;
    virtual void startBuildBlock(glm::ivec3 const& position, unsigned char blockSide) = 0;
    virtual void buildBlock(glm::ivec3 const&, unsigned char blockSide, bool) = 0;
    virtual void continueBuildBlock(glm::ivec3 const& position, unsigned char blockSide) = 0;
    virtual void stopBuildBlock() = 0;
    virtual void tick() = 0;
    virtual float getPickRange(int inputMode, bool) = 0;
    virtual void useItem(class ItemStack* targetStack) = 0;
    virtual void useItemOn(ItemStack* targetStack, glm::ivec3 const& blockPosition, int blockSide, glm::vec3 const& playerPosition, class Block const* blockClass) = 0;
    virtual void interact(class Actor* target, glm::vec3 const& position) = 0;
    virtual void attack(Actor* target) = 0;
    virtual void releaseUsingItem() = 0;

    float getDestroyRate(const class Block& block);
    bool baseUseItem(class ItemStack* itemStack);
};