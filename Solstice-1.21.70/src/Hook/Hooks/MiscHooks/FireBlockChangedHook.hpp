#pragma once
//
// Created by vastrakai on 7/28/2024.
//

#include <Hook/Hook.hpp>
#include <Utils/Structs.hpp>

class FireBlockChangedHook : public Hook
{
public:
    FireBlockChangedHook() : Hook() {
        mName = "BlockSource::fireBlockChanged";
    }

    static std::unique_ptr<Detour> mDetour;

    // symbol: void BlockSource::fireBlockChanged(class BlockPos const &,unsigned int,class Block const &,class Block const &,int,enum BlockChangedEventTarget,struct ActorBlockSyncMessage const *,class Actor *)
    static void fireBlockChangedCallback(BlockSource* self, BlockPos& blockPos, uint32_t param_2, Block* newBlock, Block* oldBlock, int param_5, int eventTarget, class ActorBlockSyncMessage* param_7, Actor* who);
    void init() override;
};

