//
// Created by vastrakai on 7/28/2024.
//

#include "FireBlockChangedHook.hpp"
#include <Features/Events/BlockChangedEvent.hpp>

std::unique_ptr<Detour> FireBlockChangedHook::mDetour = nullptr;

void FireBlockChangedHook::fireBlockChangedCallback(BlockSource* self, BlockPos& blockPos, uint32_t param_2,
    Block* newBlock, Block* oldBlock, int param_5, int eventTarget, ActorBlockSyncMessage* param_7,
    Actor* who)
{
    auto original = mDetour->getOriginal<&fireBlockChangedCallback>();
    original(self, blockPos, param_2, newBlock, oldBlock, param_5, eventTarget, param_7, who);

    auto holder = nes::make_holder<BlockChangedEvent>(blockPos, newBlock, oldBlock, eventTarget, who);
    gFeatureManager->mDispatcher->trigger(holder);
}

void FireBlockChangedHook::init()
{
    mDetour = std::make_unique<Detour>("BlockSource::fireBlockChanged", reinterpret_cast<void*>(SigManager::BlockSource_fireBlockChanged), &fireBlockChangedCallback);
}
