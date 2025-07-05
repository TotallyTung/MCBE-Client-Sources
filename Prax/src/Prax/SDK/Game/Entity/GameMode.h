//
// Created by vastrakai on 10/10/2023.
//

#pragma once

#include "Prax/SDK/Game/Misc/Structs.h"

class GameMode {
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    BUILD_ACCESS(this, Actor*, actorPtr, 0x08);
    BUILD_ACCESS(this, float, breakDuration, 0x24);

    virtual ~GameMode() = 0;
    virtual void _startDestroyBlock(BlockPos* position, int blockSide, bool& isDestroyedOut) = 0;
    virtual void _destroyBlock(BlockPos* position, int blockSide) = 0;
    virtual void _continueDestroyBlock(BlockPos const& position, unsigned int blockSide, Vector3 const& playerPosition, bool&) = 0;
    virtual void stopDestroyBlock(BlockPos const& position) = 0;
    virtual void startBuildBlock(BlockPos const& position, unsigned char blockSide) = 0;
    virtual void buildBlock(BlockPos const&, unsigned char blockSide, bool shouldCrash) = 0;
    virtual void continueBuildBlock(BlockPos const& position, unsigned char blockSide) = 0;
    virtual void stopBuildBlock(void) = 0;
    virtual void tick(void) = 0;
    virtual float getPickRange(int inputMode, bool) = 0;
    virtual void useItem(class ItemStack* targetStack) = 0;
    virtual void useItemOn(ItemStack* targetStack, BlockPos const& blockPosition, int blockSide, Vector3 const& playerPosition, Block const* blockClass) = 0;
    virtual void interact(Actor* target, Vector3 const& position) = 0;
    virtual void attack(Actor* target) = 0;
    virtual void releaseUsingItem(void) = 0;
    virtual void setTrialMode(bool) = 0;
    virtual bool isInTrialMode(void) = 0;

    // This is to prevent calls from prax from interfering with PacketMine and causing a crash
    void startDestroyBlock(BlockPos* position, int blockSide, bool& isDestroyedOut)
    {
        _startDestroyBlock(position, blockSide - 6, isDestroyedOut);
    }

    void destroyBlock(BlockPos* position, int blockSide)
    {
        _destroyBlock(position, blockSide - 6);
    }

    void continueDestroyBlock(BlockPos const& position, unsigned int blockSide, Vector3 const& playerPosition, bool& a4)
    {
        _continueDestroyBlock(position, blockSide - 6, playerPosition, a4);
    }

    // (non-static)
    float getDestroyRate(Block const& block){
        using getDestroyRate = float(__fastcall*)(GameMode*, Block const&);
        static getDestroyRate getDestroyRateFunc = reinterpret_cast<getDestroyRate>(SigManager::GetSignatureAddress(SignatureEnum::Gamemode_getDestroyRate));
        return getDestroyRateFunc(this, block);
    }

    bool baseUseItem(ItemStack* stack) {
        using baseUseItem = bool(__fastcall*)(GameMode*, ItemStack*);
        static baseUseItem baseUseItemFunc = reinterpret_cast<baseUseItem>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Gamemode_baseUseItem)));

        if (!stack) return false;
        return baseUseItemFunc(this, stack);
    }
};